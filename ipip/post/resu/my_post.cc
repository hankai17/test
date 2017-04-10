#include<stdlib.h>
#include<curl/curl.h>
#include<string.h>
#include<fcntl.h>
#include<iostream>
#include<string>

#define MAX_BUF 1024*1024

std::string buffer;

int write_data(char* page_data,size_t size, size_t nmemb,std::string *data)
{
    if(data == NULL)
        return 0;
    data->append(page_data);
    return size * nmemb;
}
int parse(std::string buffer)
{
   std::string location = buffer.substr(buffer.find("<span id=\"myself\">")+strlen("<span id=\"myself\">"), buffer.find("<",buffer.find("<span id=\"myself\">")+strlen("<span id=\"myself\">"))-(buffer.find("<span id=\"myself\">")+strlen("<span id=\"myself\">")));                                                                                        
   std::cout<<"location="<<location<<std::endl;

   return 0;
}








int main()
{
    char post_data[MAX_BUF] = {0};
    int len;
    //strcpy(post_data,"ip=114.114.114.19");
    //strcpy(post_data,"ip=123.120.181.165");
    //strcpy(post_data,"ip=1.1.1.0");
    strcpy(post_data,"ip=201.0.181.0");
    len = strlen(post_data);

    char User_Agent[1024*1024] = {0};
    memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
            strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));
    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, User_Agent);

    CURLcode ret;
    CURL* curl = curl_easy_init();
    if(!curl) 
        return 1;
    //content_type application/x-www-form-ulencoded header
    curl_easy_setopt(curl,CURLOPT_POST,1L);
    curl_easy_setopt(curl,CURLOPT_URL,"http://www.ipip.net/ip.html");

    curl_easy_setopt(curl,CURLOPT_REFERER,"http://www.ipip.net/ip.html");
    //posting data
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);

    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    //write to buffer
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,&buffer);

    curl_easy_perform(curl);

    //std::cout<<"buffer="<<buffer<<std::endl;
    parse(buffer);

    return 0; 
}


