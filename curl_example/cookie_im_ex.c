#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <curl/curl.h>

static void print_cookies(CURL* curl) {
  CURLcode res;
  struct curl_slist* cookies;
  struct curl_slist* nc;
  int i;

  printf("Cookies, curl knows:\n");
  res = curl_easy_getinfo(curl, CURLINFO_COOKIELIST, &cookies);
  if(res != CURLE_OK) {
	fprintf(stderr, "Curl curl_easy_getinfo failed: %s\n", curl_easy_strerror(res));
	exit(1);
  }
  nc = cookies;
  i = 1;
  while(nc) {
	printf("[%d]: %s\n", i, nc->data);
	nc = nc->next;
	i++;
  }
  if(i == 1) {
	printf("(none)\n");
  }
  curl_slist_free_all(cookies);
}

int main(void) {
  CURL* curl;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if(curl) {
	char nline[256];

	curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com/");
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(curl, CURLOPT_COOKIEFILE, ""); /* start cookie engine */
	res = curl_easy_perform(curl);
	if(res != CURLE_OK) {
	  fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
	  return 1;
	}
	print_cookies(curl); //打印cookie
	printf("Erasing curl's knowledge of cookies!\n");
	curl_easy_setopt(curl, CURLOPT_COOKIELIST, "ALL"); //情况cookie
	print_cookies(curl); //cookie为空
	printf("-----------------------------------------------\n" "Setting a cookie \"PREF\" via cookie interface:\n");
	/* Netscape format cookie */
	snprintf(nline, sizeof(nline), "%s\t%s\t%s\t%s\t%lu\t%s\t%s",
		  ".example.com", "TRUE", "/", "FALSE",
		  (unsigned long)time(NULL) + 31337UL,
		  "PREF", "hello example, i like you very much!");
	res = curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline); //设置cookie
	if(res != CURLE_OK) {
	  fprintf(stderr, "Curl curl_easy_setopt failed: %s\n", curl_easy_strerror(res));
	  return 1;
	}
	print_cookies(curl);

	/* HTTP-header style cookie. If you use the Set-Cookie format and don't
	   specify a domain then the cookie is sent for any domain and will not be
	   modified, likely not what you intended. Starting in 7.43.0 any-domain
	   cookies will not be exported either. For more information refer to the
	   CnURLOPT_COOKIELIST documentation.
	 */
/*
	snprintf(nline, sizeof(nline),
		  "Set-Cookie: OLD_PREF=3d141414bf4209321; "
		  "expires=Sun, 17-Jan-2038 19:14:07 GMT; path=/; domain=.example.com");
	res = curl_easy_setopt(curl, CURLOPT_COOKIELIST, nline);
	if(res != CURLE_OK) {
	  fprintf(stderr, "Curl curl_easy_setopt failed: %s\n",
			curl_easy_strerror(res));
	  return 1;
	}

	print_cookies(curl);

	res = curl_easy_perform(curl);
	if(res != CURLE_OK) {
	  fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
	  return 1;
	}

*/
	curl_easy_cleanup(curl);
  } else {
	fprintf(stderr, "Curl init failed!\n");
	return 1;
  }
  curl_global_cleanup();
  return 0;
}
