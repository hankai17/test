#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <curl/curl.h>

int main(void) {
  CURL *http_handle;
  CURL *http_handle2;
  CURLM *multi_handle;
  int still_running; /* keep number of running handles */

  http_handle = curl_easy_init();
  http_handle2 = curl_easy_init();
  multi_handle = curl_multi_init();

  curl_easy_setopt(http_handle, CURLOPT_URL, "http://www.example.com/");
  curl_easy_setopt(http_handle2, CURLOPT_URL, "http://localhost/");
  curl_multi_add_handle(multi_handle, http_handle);
  curl_multi_add_handle(multi_handle, http_handle2);
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
      if(timeout.tv_sec > 1)
        timeout.tv_sec = 1;
      else
        timeout.tv_usec = (curl_timeo % 1000) * 1000;
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
      rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout); //为什么需要select 难道curl内部不会监听fd  
																	 //我监听的作用就是为了防止忙轮询  但是非常奇怪的是为什么要把fds传出来 又不用他 为什么?
    }

    switch(rc) {
    case -1:
      /* select error */
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
  curl_easy_cleanup(http_handle2);

  return 0;
}

//这个做法是典型的 我来做监听 然后perform
