
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <curl/curl.h>

//multi interface and debug callback, Simply download a HTTP file.
typedef char bool;
#define TRUE 1

static void dump(const char *text, FILE *stream, unsigned char *ptr, size_t size, bool nohex) {
  size_t i;
  size_t c;

  unsigned int width = 0x10;

  if(nohex)
  /* without the hex output, we can fit more on screen */
  width = 0x40;

  fprintf(stream, "%s, %10.10lu bytes (0x%8.8lx)\n",
		text, (unsigned long)size, (unsigned long)size);

  for(i = 0; i<size; i += width) {

	fprintf(stream, "%4.4lx: ", (unsigned long)i);

	if(!nohex) {
	  /* hex not disabled, show it */
	  for(c = 0; c < width; c++)
	  if(i + c < size)
	  fprintf(stream, "%02x ", ptr[i + c]);
	  else
	  fputs("   ", stream);
	}

	for(c = 0; (c < width) && (i + c < size); c++) {
	  /* check for 0D0A; if found, skip past and start a new line of output */
	  if(nohex && (i + c + 1 < size) && ptr[i + c] == 0x0D &&
			ptr[i + c + 1] == 0x0A) {
		i += (c + 2 - width);
		break;
	  }
	  fprintf(stream, "%c",
			(ptr[i + c] >= 0x20) && (ptr[i + c]<0x80)?ptr[i + c]:'.');
	  /* check again for 0D0A, to avoid an extra \n if it's at width */
	  if(nohex && (i + c + 2 < size) && ptr[i + c + 1] == 0x0D &&
			ptr[i + c + 2] == 0x0A) {
		i += (c + 3 - width);
		break;
	  }
	}
	fputc('\n', stream); /* newline */
  }
  fflush(stream);
}

static int my_trace(CURL *handle, curl_infotype type, unsigned char *data, size_t size, void *userp) {
  const char *text;
  (void)userp;
  (void)handle; /* prevent compiler warning */

  switch(type) {
	case CURLINFO_TEXT:
	  fprintf(stderr, "== Info: %s", data);
	  /* FALLTHROUGH */
	default: /* in case a new one is introduced to shock us */
	  return 0;

	case CURLINFO_HEADER_OUT:
	  text = "=> Send header";
	  break;
	case CURLINFO_DATA_OUT:
	  text = "=> Send data";
	  break;
	case CURLINFO_HEADER_IN:
	  text = "<= Recv header";
	  break;
	case CURLINFO_DATA_IN:
	  text = "<= Recv data";
	  break;
  }

  //dump(text, stderr, data, size, TRUE);
  printf("            trace:%s\n", text);
  return 0;
}

int main(void) {
  CURL* http_handle;
  CURLM* multi_handle;
  int still_running;
  http_handle = curl_easy_init();
  curl_easy_setopt(http_handle, CURLOPT_URL, "http://www.baidu.com/");
  curl_easy_setopt(http_handle, CURLOPT_DEBUGFUNCTION, my_trace);
  curl_easy_setopt(http_handle, CURLOPT_VERBOSE, 1L);

  multi_handle = curl_multi_init();
  curl_multi_add_handle(multi_handle, http_handle);
  curl_multi_perform(multi_handle, &still_running);

  do {
	struct timeval timeout;
	int rc; /* select() return code */
	CURLMcode mc; /* curl_multi_fdset() return code */

	fd_set fdread;
	fd_set fdwrite;
	fd_set fdexcep;
	int maxfd = -1;
	long curl_timeo = -1;
	FD_ZERO(&fdread);
	FD_ZERO(&fdwrite);
	FD_ZERO(&fdexcep);

	/* set a suitable timeout to play around with */
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	curl_multi_timeout(multi_handle, &curl_timeo);
	if(curl_timeo >= 0) {
	  timeout.tv_sec = curl_timeo / 1000;
	  if(timeout.tv_sec > 1) timeout.tv_sec = 1;
	  else timeout.tv_usec = (curl_timeo % 1000) * 1000;
	}

	/* get file descriptors from the transfers */
	mc = curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);
	if(mc != CURLM_OK) {
	  fprintf(stderr, "curl_multi_fdset() failed, code %d.\n", mc);
	  break;
	}

	/* On success the value of maxfd is guaranteed to be >= -1. We call
	   select(maxfd + 1, ...); specially in case of (maxfd == -1) there are
	   no fds ready yet so we call select(0, ...) --or Sleep() on Windows--
	   to sleep 100ms, which is the minimum suggested value in the
	   curl_multi_fdset() doc. */

	if(maxfd == -1) {
	  /* Portable sleep for platforms other than Windows. */
	  struct timeval wait = { 0, 100 * 1000 }; /* 100ms */
	  rc = select(0, NULL, NULL, NULL, &wait);
	} else {
	  /* Note that on some platforms 'timeout' may be modified by select().
		 If you need access to the original value save a copy beforehand. */
	  rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
	}

	switch(rc) {
	  case -1:
		/* select error */
		still_running = 0;
		printf("select() returns error, this is badness\n");
		break;
	  case 0:
	  default:
		/* timeout or readable/writable sockets */
		curl_multi_perform(multi_handle, &still_running);
		break;
	}
  } while(still_running);
  curl_multi_cleanup(multi_handle);
  curl_easy_cleanup(http_handle);
  return 0;
}
