#include <stdio.h>
#include <curl/curl.h>

int main(void) {
  CURL* curl;
  CURLcode res;
  curl = curl_easy_init();
  if(curl) {
	struct curl_slist* chunk = NULL;

	/* Remove a header curl would otherwise add by itself */
	chunk = curl_slist_append(chunk, "Accept:"); //什么都不写即 删除此首部

	/* Add a custom header */
	chunk = curl_slist_append(chunk, "Another: yes");

	/* Modify a header curl otherwise adds differently */
	chunk = curl_slist_append(chunk, "Host: example.com");

	/* Add a header with "blank" contents to the right of the colon. Note that
	   we're then using a semicolon in the string we pass to curl! */
	chunk = curl_slist_append(chunk, "X-silly-header;");

	/* set our custom set of headers */
	res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

	curl_easy_setopt(curl, CURLOPT_URL, "localhost");
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK) fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

	/* always cleanup */
	curl_easy_cleanup(curl);

	/* free the custom headers */
	curl_slist_free_all(chunk);
  }
  return 0;
}
//HTTP request with custom modified, removed and added headers
