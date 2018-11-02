#include <stdio.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void) {
  CURL* curl;
  CURLcode res;
  struct stat file_info;
  double speed_upload, total_time;
  FILE* fd;

  fd = fopen("debugit", "rb"); /* open file to upload */
  if(!fd) return 1;

  /* to get the file size */
  if(fstat(fileno(fd), &file_info) != 0)
  return 1;

  curl = curl_easy_init();
  if(curl) {
	curl_easy_setopt(curl, CURLOPT_URL, "file:///home/dast/src/curl/debug/new"); //upload place

	/* tell it to "upload" to the URL */
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

	/* set where to read from */
	curl_easy_setopt(curl, CURLOPT_READDATA, fd);

	/* and give the size of the upload (optional) */
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);

	/* enable verbose for easier tracing */
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	res = curl_easy_perform(curl);
	/* Check for errors */
	if(res != CURLE_OK) {
	  fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));

	} else {
	  /* now extract transfer info */
	  curl_easy_getinfo(curl, CURLINFO_SPEED_UPLOAD, &speed_upload);
	  curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &total_time);

	  fprintf(stderr, "Speed: %.3f bytes/sec during %.3f seconds\n", speed_upload, total_time);
	}
	/* always cleanup */
	curl_easy_cleanup(curl);
  }
  fclose(fd);
  return 0;
}
