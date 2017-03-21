#include<iostream>
#include<string.h>
#include<curl/curl.h>
#include <unistd.h>  

#define NO '0'
#define YES '1'
#define FILE_NAME "curl.html"
/*
size_t write_data(__attribute__((unused)) void *buffer, size_t size, size_t nmemb, __attribute__((unused)) void *user_p)
{
    return size * nmemb;
}
*/

static size_t write_data(void *ptr, size_t size, size_t nmemb,void *stream)
{
    int len = size * nmemb;  
    int written = len;  
    FILE *fp = NULL;  
    if (access((char*) stream, 0) == -1) {  
        fp = fopen((char*) stream, "wb");  
    } else {  
        fp = fopen((char*) stream, "ab");  
    }  
    if (fp) {  
        fwrite(ptr, size, nmemb, fp);  
    }  
    // printf("%s\n",ptr);  
    fclose(fp);  
    return written;  
}  

char curl_ics(std::string &chip)
{
    char unRet = NO;
    long code;

    char url[1024] =  {0}; 
	char User_Agent[500] = {0};
	memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
               strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));

    memcpy(url, "http://freeapi.ipip.net/118.28.8.8", strlen("http://freeapi.ipip.net/118.28.8.8"));

    CURL * curl = curl_easy_init(); //初始化

	struct curl_slist *slist = NULL;
	slist = curl_slist_append(slist, User_Agent);

    if (curl != NULL)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0); //一并获取响应头
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 0); //Get 请求
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //receive data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,FILE_NAME); // write_data param

        unRet = curl_easy_perform(curl);
        if (CURLE_OK == unRet)
        {
            unRet = YES;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
            if (code >= 400)
            {
                unRet = NO;
            }
            std::cout<<chip<<" response code: "<<code<<std::endl;
        }
        else
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
            std::cout<<chip<<" response code: "<<code<<std::endl;
            unRet = NO;
        }
        curl_easy_cleanup(curl);
    }
    return unRet;
}

int main()
{
    std::string chip("ip138");
    curl_ics(chip);
    return 0;
}

