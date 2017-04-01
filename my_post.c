#include<stdlib.h>
#include<curl/curl.h>
#include<string.h>
#include<fcntl.h>
#include<iostream>
#include<string>

std::string buffer;

int write_data(char* page_data,size_t size, size_t nmemb,std::string *data)
{
    if(data == NULL)
        return 0;
    data->append(page_data);
    return size * nmemb;
}

int main()
{
	char post_data[MAX_BUF] = {0};
	int len;
    strcpy(post_data,"ip:\"114.114.114.114\"");
    len = strlen(post_data);

    CURLcode ret;
    CURL* curl = curl_easy_init();
    if(!curl) 
        return 1;
    //content_type application/x-www-form-ulencoded header
    curl_easy_setopt(curl,CURLOPT_POST,1L);
    curl_easy_setopt(curl,CURLOPT_URL,"http://www.ipip.net/ip.html");
    //posting data
	curl_easy_setopt(curl,CURLOPT_POSTFIELDS, post_data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);

    //write to buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,&buffer);

    curl_easy_perform(curl);

    std::cout<<"buffer="<<buffer<<std::endl;

   return 0; 
}
