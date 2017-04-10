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

int parse(std::string buffer)
{
    boost::regex::flag_type flag;
    flag = boost::regex::perl;
    flag |= boost::regex::no_except;

    std::string regex;
    regex = "(\\d{3}[\\.]){3}";

    const boost::regex e(regex, flag);
    if (e.status()) 
    {
        std::cout << "Incorrect regex pattern!\n";
        return 0;
    }
    boost::smatch result;
    bool matched = boost::regex_search(buffer, result, e); 
    std::cout << (matched ? "MATCH\n" : "DOES NOT MATCH\n");
    std::cout<<"matching str="<<result[0]<<std::endl;
    
    /*
    std::string location = buffer.substr(buffer.find("<span id=\"myself\">")+strlen("<span id=\"myself\">"), buffer.find("<",buffer.find("<span id=\"myself\">")+strlen("<span id=\"myself\">"))-(buffer.find("<span id=\"myself\">")+strlen("<span id=\"myself\">")));                                                                                        
    std::string ip1 = buffer.substr(buffer.find("您输入的IP: <span style=\"color: rgb(243, 102, 102);\">")+strlen("您输入的IP: <span style=\"color: rgb(243, 102, 102);\">"), buffer.find("<",buffer.find("您输入的IP: <span style=\"color: rgb(243, 102, 102);\">")+strlen("您输入的IP: <span style=\"color: rgb(243, 102, 102);\">"))-(buffer.find("您输入的IP: <span style=\"color: rgb(243, 102, 102);\">")+strlen("您输入的IP: <span style=\"color: rgb(243, 102, 102);\">")));                                                                                        
    std::cout<<"location="<<location<<std::endl;
    std::cout<<"ip1="<<ip1<<std::endl;
    */
    return 0;
}

char* int_2_str(int ip, char *buf)  
{  
    sprintf(buf, "%u.%u.%u.%u",  
            (unsigned char )*((char *)&ip + 3),  
            (unsigned char )*((char *)&ip + 2),  
            (unsigned char )*((char *)&ip + 1),  
            (unsigned char )*((char *)&ip + 0));  
    //std::cout<<"buf="<<buf<<std::endl;
    return buf;  
}  

void confirm_data(char* ip_data)
{
        unsigned long ip_long = inet_network(ip_data);
        ip_long += 256;
        
        char tmp[32];
        int_2_str(ip_long,tmp);
        std::string new_ip = {tmp};

        memset(ip_data,0,16);
        strcpy(ip_data,(char*)new_ip.data());
}

int main()
{
    char post_data[1024] = {0};
    std::string buffer = "";
    int len;
    char ip[16] = "123.120.167.25";

    char User_Agent[1024*1024] = {0};
    memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
            strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));
    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, User_Agent);
    CURLcode ret;

    std::chrono::milliseconds sleepDuration(5000);
    while(1)
    {
        confirm_data(ip);
        sprintf(post_data,"ip=%s",ip);
        std::cout<<"post_data="<<post_data<<std::endl;
        len = strlen(post_data);
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
        std::this_thread::sleep_for(sleepDuration);
        buffer = "";
    }

    return 0; 
}


