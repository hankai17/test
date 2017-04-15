#include<stdlib.h>
#include<curl/curl.h>
#include<string.h>
#include<fcntl.h>
#include<iostream>
#include<stdio.h>
#include<string>
#include<unistd.h>  
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<chrono>
#include<thread>
#include <boost/regex.hpp>

int write_data(char* page_data,size_t size, size_t nmemb,std::string *data)
{
    if(data == NULL)
        return 0;
    data->append(page_data);
    return size * nmemb;
}
void get_keystring(std::string buf,char* qip)  
{
    char* tmp = (char*)(buf.data());
    char* p = tmp;
    int blank_flag = 0;
    int freq = 0;
    while(*p)
    {
        if(*p == ' ')
        {
            blank_flag = 1; 
            p++;
            continue;
        }
        if(1 == blank_flag)
        {
            freq++;
            blank_flag = 0;
        }
        if(freq == 2)
        {
            //std::cout<<"p="<<p<<std::endl;
            strcpy(qip,p);
            break; 
        }
        p++;
    }
    return;
}

int parse(std::string buffer)
{
    boost::regex::flag_type flag;
    flag = boost::regex::perl;
    flag |= boost::regex::no_except;

    std::string regex;
    regex = "span id=\"myself\">[^<]*";
    const boost::regex e(regex, flag);
    if (e.status()) 
    {
        std::cout << "error regex pattern!\n";
        return 0;
    }
    boost::smatch result;
    bool matched = boost::regex_search(buffer, result, e); 
    //std::cout << (matched ? "match!\n" : "not match\n");
    std::cout << (matched ? "" : "not match\n");
    char qip[1024] = {0};
    get_keystring(result[0],qip);  
    std::cout<<qip<<std::endl;
    return 0;
}


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("use: ./qip ip \n eg:\n  ./qip 114.114.114.114\n");
        return 0;
    }
    char ip[16] = {0};
    char post_data[1024] = {0};
    char ret;
    int code;
    std::string buffer = "";
    strcpy(ip,argv[1]);
    sprintf(post_data,"ip=%s",ip);
    int len = 0;
    len = strlen(post_data);

    //std::cout<<"post_data="<<post_data<<std::endl;
    char User_Agent[1024*1024] = {0};
    memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
            strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));
    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, User_Agent);

    CURL* curl = curl_easy_init();
    if(!curl) 
        return 1;
    curl_easy_setopt(curl,CURLOPT_POST,1L);
    curl_easy_setopt(curl,CURLOPT_URL,"http://www.ipip.net/ip.html");
    curl_easy_setopt(curl,CURLOPT_REFERER,"http://www.ipip.net/ip.html");
    curl_easy_setopt(curl,CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA,&buffer);
    ret = curl_easy_perform(curl);
    if(ret == CURLE_OK)
    {
        curl_easy_getinfo(curl,CURLINFO_RESPONSE_CODE,&code);
        if(code != 200)
        {
            std::cout<<"err, http response code = "<<code<<std::endl; 
            curl_easy_cleanup(curl);
            return 0;
        }
    }
    else
    {
        std::cout<<"curl error"<<std::endl;
        curl_easy_cleanup(curl);
        return 0;
    }

    parse(buffer);
    curl_easy_cleanup(curl);
    return 0;

}
