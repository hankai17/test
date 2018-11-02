#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <curl/curl.h>

#define WAITMS(x)                               \
  struct timeval wait = { 0, (x) * 1000 };      \
  (void)select(0, NULL, NULL, NULL, &wait);

int main(void) {
  CURL* http_handle;
  CURLM* multi_handle;

  int still_running; /* keep number of running handles */
  int repeats = 0;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  http_handle = curl_easy_init();

  curl_easy_setopt(http_handle, CURLOPT_URL, "http://www.example.com/");

  /* init a multi stack */
  multi_handle = curl_multi_init();
  curl_multi_add_handle(multi_handle, http_handle);
  curl_multi_perform(multi_handle, &still_running);

  do {
    CURLMcode mc; /* curl_multi_wait() return code */
    int numfds;

    /* wait for activity, timeout or "nothing" */
    mc = curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
    if(mc != CURLM_OK) {
      fprintf(stderr, "curl_multi_wait() failed, code %d.\n", mc);
      break;
    }

    /* 'numfds' being zero means either a timeout or no file descriptors to
       wait for. Try timeout on first occurrence, then assume no file
       descriptors and no file descriptors to wait for means wait for 100
       milliseconds. */

    if(!numfds) {
      repeats++; /* count number of repeated zero numfds */
      if(repeats > 1) {
        WAITMS(100); /* sleep 100 milliseconds */ //主要防止忙读 忙轮询
      }
    } else repeats = 0;
    curl_multi_perform(multi_handle, &still_running);
  } while(still_running);

  curl_multi_remove_handle(multi_handle, http_handle);
  curl_easy_cleanup(http_handle);
  curl_multi_cleanup(multi_handle);
  curl_global_cleanup();
  return 0;
}
