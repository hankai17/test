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

#define MAX_BUF 1024*1024


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
    /*
       for(int i = 0; i < location.length(); i++)
       {
    //printf("%c == %d \n",(location.c_str())[i],(location.c_str())[i]);
    printf("%c == %d \n",((char*)(location.data()))[i],((char*)(location.data()))[i]);
    }
    */
    return 0;
}

int main()
{
        std::string buffer;
        std::string ip_tmp = "201.0.181.0";
        unsigned long ip_long;
        char post_data[MAX_BUF] = {0};
        int len;

    for(;;) 
    {

        strcpy(post_data,ip_tmp.c_str());
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
        std::cout<<"post_data="<<post_data<<std::endl;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, len);

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

        //write to buffer
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,&buffer);

        curl_easy_perform(curl);

        std::cout<<"buffer="<<buffer<<std::endl;
        parse(buffer);


        sleep(20);
        ip_long = inet_network(ip_tmp.data());
        ip_long++;
        char tmp[32];
        int_2_str(ip_long,tmp); 
        ip_tmp = "";
        ip_tmp = {tmp};
        std::cout<<"ip="<<ip_tmp;

    }
    return 0; 
}
