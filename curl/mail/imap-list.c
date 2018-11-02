#include <stdio.h>
#include <curl/curl.h>

//This is a simple example showing how to list the folders within an IMAP mailbox.

int main(void) {
  CURL* curl;
  CURLcode res = CURLE_OK;

  curl = curl_easy_init();
  if(curl) {
	curl_easy_setopt(curl, CURLOPT_USERNAME, "hankai17");
	curl_easy_setopt(curl, CURLOPT_PASSWORD, "8991406");

	/* This will list the folders within the user's mailbox. If you want to
	 * list the folders within a specific folder, for example the inbox, then
	 * specify the folder as a path in the URL such as /INBOX */
	curl_easy_setopt(curl, CURLOPT_URL, "imap://imap.126.com/");
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,"LIST");
	res = curl_easy_perform(curl);
	if(res != CURLE_OK)
	fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	curl_easy_cleanup(curl);
  }

  return (int)res;
}
