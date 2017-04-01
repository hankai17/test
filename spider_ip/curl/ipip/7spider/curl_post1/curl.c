#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <fcntl.h>

#define MAX_BUF 655300
#define SUCEED_OK 0
#define ERROR_CURL_INITIALFALIED -1
#define ERROR_CURL_TIMEOUT -2
#define ERROR_CURL_HTTPFALSE -3
#define ERROR_CURL_SSLINITFAILED -4
#define ERROR_FAILED_SEND -5
#define HEAD_KEEP 0
#define PCM_FILE "/home/jim/test/spotify/goertek/python/TomCruise.pcm"

typedef size_t (*func)(void *buffer, size_t size, size_t nmemb, char *userp);

/*buffer is curl_wasy_setopt(,,wr_buf), for keep data*/
size_t load_data( void *buffer, size_t size, size_t nmemb, char *userp ) 
{
	int  wr_index = 0; 

	size_t segsize = size * nmemb;

	if ( wr_index + segsize > MAX_BUF ) { 

		userp[0] = 0; 

		return 0; 
	} 

	/* copy the data from the curl buffer into our buffer */ 
	memcpy(userp, buffer, segsize ); 

	/* update the write index */ 
	wr_index += segsize; 

	/* null terminate the buffer */ 
	userp[wr_index] = 0;
#if 0
	printf(">>%s", userp);
#endif
	char *tran = strstr(userp, "transcriptions");
	if (tran == NULL) {
		return 0;
	} else {
		int i = 0;
		char bf[4096] = {0};
		for (i = 17;  tran[i] != ']'; i++) {
			bf[i-17] = tran[i];
		}
		printf(">>%s\n", bf);
	}

	/* return the number of bytes received, indicating to curl that all is okay */ 
	return segsize; 
} 

/*-----------------------------------------------------------------------------
 *func:receive the header data from the server and save it to the file stream
 *
 *@param stream :the temp file used to save the header data receive from the server
 *
 *return the size of the string receive
  *---------------------------------------------------------------------------*/
#if HEAD_KEEP
size_t load_header( void *buffer, size_t size, size_t nmemb, FILE *stream ) 
{
	/*write receive http header to stream*/
	return fwrite(buffer, size, nmemb, stream);
}
#endif

int send_https(const char *url,
		struct curl_slist *http_headers,
		const char *poststring,
		unsigned long timeout,
		char * wr_buf,
		int len,
		func callback)
{
	CURLcode ret;
	int error_code=SUCEED_OK;
	CURL *curl = curl_easy_init();
	if (!curl)  return ERROR_CURL_INITIALFALIED;
	/*set request header*/
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, http_headers);
	/*open ssl protol*/
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);	
	
	/*set url*/ 
	curl_easy_setopt(curl,CURLOPT_URL, url);
	
	/*set post string*/
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS, poststring);

	/*must set CURLOPT_POSTFIELDSIZE, other will send poststring as string*/
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);

	/*set timetou to connect server*/
	curl_easy_setopt(curl,CURLOPT_CONNECTTIMEOUT, timeout);

	/*wr_buf is the param for callback function to keep data*/ 
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, wr_buf); 
	/*callback for receive data from server*/
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
#if HEAD_KEEP
	static const char *headerfilename = "head.out";  
	FILE *headerfile;
	headerfile = fopen(headerfilename, "wb");
	if (headerfile == NULL) {
		printf("over......\n");
		return -1;
	}

	/*set callback for header*/
	curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, load_header);
	/*headerfile receive header responses*/
	curl_easy_setopt(curl,CURLOPT_WRITEHEADER, headerfile);
#endif
	ret = curl_easy_perform(curl);

	if ( ret == 0 ) {
		printf("success send\n");
	} else {
		switch(ret) {
		case CURLE_COULDNT_CONNECT:
			printf("ERROR:TimeOut can't connect to the host.\n");
			error_code=ERROR_CURL_TIMEOUT;
			break;
		case CURLE_HTTP_RETURNED_ERROR:
			printf("ERROR:HTTP return false.");
			error_code=ERROR_CURL_HTTPFALSE;
			break;
		case CURLE_SSL_ENGINE_INITFAILED:
			printf("ERROR:SSL can't be initialized.");
			error_code=ERROR_CURL_SSLINITFAILED;
			break;
		case CURLE_COULDNT_RESOLVE_HOST:
			printf("ERROR:SSL can't create ssl connection.");
			error_code=ERROR_CURL_SSLINITFAILED;
			break;
		default:
			printf("ERROR:correspond failed.");
			error_code=ERROR_FAILED_SEND;
		}
	}

#if HEAD_KEEP
	fclose(headerfile);
#endif
	return error_code;
}

static char *request_data = "{\
        \"appKey\":\"9acf33fb7520598c5729de7eeaad3141b7fed01fd89f227ff12a826761749e42b3ee02e35a4defc99d6d71a4d072b017c43085bf80cc507430a308ce5d6a93eb\",\
	\"appId\":\"GOERTEK_EVAL_20150601\",\
	\"uId\":\"1234567890123456789012345678901234567890\",\
	\"inCodec\":\"PCM_16_16K\",\
	\"outCodec\":\"PCM_16_16K\",\
	\"cmdName\":\"DRAGON_NLU_ASR_CMD\",\
#	\"cmdName\":\"NVC_ASR_CMD\",\
	\"appName\":\"ddpasrproxy\",\
	\"appVersion\":\"1.0\",\
	\"language\":\"eng-USA\",\
	\"carrier\":\"carrier\",\
	\"deviceModel\":\"UNKNOWN\",\
	\"cmdDict\": {\
		\"dictation_type\":\"DTV\",\
#		\"dictation_type\":\"Dictation\",\
		\"dictation_language\":\"eng-USA\",\
		\"application_name\":\"ddpasrproxy\",\
		\"organization_id\":\"organization name\",\
		\"application_session_id\":\"1234567890\",\
		\"utterance_number\":\"5\",\
		\"ui_language\":\"en\",\
		\"application_state_id\":\"\"\
	}\
}";

char *request_info = "{\
	\"start\": 0,\
	\"end\": 0,\
	\"text\": \"text present on the device screen\",\
#        \"appserver_data\" : {\
#        }\
}";

/*boundary may be uuid's function*/
char *boundary = "2e38f6ea8e704efd9b625c30c7100d90";
char *head_data = "Content-Disposition: form-data; name=\"RequestData\"\r\nContent-Type: application/json; charset=utf-8\r\n\r\n";
char *head_info = "Content-Disposition: form-data; name=\"DictParameter\"; paramName=\"REQUEST_INFO\"\r\nContent-Type: application/json; charset=utf-8\r\n\r\n";
char *head_audio = "Content-Disposition: form-data; name=\"ConcludingAudioParameter\"; paramName=\"AUDIO_INFO\"\r\nContent-Type: audio/x-wav;codec=pcm;bit=16;rate=16000\r\n\r\n";

/*joint two memory, plesae attention, be sure dst memory is enough to include src*/
int memncpy(char *dst, char *src, int dst_position, int src_len)
{
	if ((dst == NULL) || (src == NULL)) {
		printf("error: %s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		return -1;
	}

	int i;
	for (i = 0; i < src_len; i++) {
		dst[dst_position] = src[i];
		dst_position++;
	}

	return 0;
}

int post(char *pcm_file, func callback)
{
	char wr_buf[MAX_BUF] = {0};
	char buf[MAX_BUF] = {0};
	if ((pcm_file == NULL) && (callback == NULL)) {
		printf("please confirm pcm file and callback exit\n");
		return -1;
	}

	char fd_buf[1000000] = {0};
	sprintf(buf, "--%s\r\n%s%s\r\n", boundary, head_data, request_data);
	strcat(wr_buf, buf);
	/*-------------------------------------------------------------------------*/
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "--%s\r\n%s%s\r\n", boundary, head_info, request_info);
	strcat(wr_buf, buf);

	/*-------------------------------------------------------------------------*/
	int rc, num;
	int len;
	/*open file in the format of binary*/
	FILE *fpin = fopen(pcm_file, "rb");
	if (fpin == NULL) {
		printf("open error, format: ./curl_post file\n");
		return 0;
	}

	while ((rc = fread(fd_buf, 1, 1000000, fpin)) != 0) {
		memset(buf, 0, MAX_BUF);
		
		sprintf(buf, "--%s\r\n%s", boundary, head_audio);
		strcat(wr_buf, buf);
		fd_buf[rc-1] = '\r';
		fd_buf[rc] = '\n';
		len = strlen(wr_buf);
		num = rc + 1;
		/*attention: do not use snprintf, because snprintf joint two string*/
		memncpy(wr_buf, fd_buf, len, num);
#if 0
		int i;
		for (i = 0; i < num + len; i++)
			printf("%02x", wr_buf[i]);
#endif
	}
	fclose(fpin);

	/*-------------------------------------------------------------------------*/
	/*add boundary*/
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "--%s--\r\n", boundary);
	memncpy(wr_buf, buf, len + num, strlen(buf));
	len = len + num + strlen(buf);
	/*-------------------------------------------------------------------------*/

	/*init http head*/
	struct curl_slist *headers = NULL;
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "Content-Type: multipart/form-data; boundary=%s", boundary);
	headers = curl_slist_append(headers, buf);
	headers = curl_slist_append(headers, "Connection: Keep-Alive");
	headers = curl_slist_append(headers, "Content-Type: multipart/form-data");
	memset(buf, 0, MAX_BUF);
	sprintf(buf, "boundary=%s\r\n\r\n", boundary);
	headers = curl_slist_append(headers, buf);
	send_https("https://goertek-ncs-engusa-ssl.nuancemobility.net/NmspServlet/",
			headers,
			wr_buf,
			100,
			wr_buf,
			len,
			callback);
	return 0;
}

int main(int argc, char *argv[])
{
	post(argv[1], load_data);
	return 0;
}
