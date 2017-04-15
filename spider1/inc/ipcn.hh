#pragma once
#include"spider_ip.hh" 

#define FILE_NAME "curl.html"

class ipcn_spider_ip:public spider_ip
{
    public:
        ipcn_spider_ip(std::string ip_b, std::string ip_e);
        static int write_data(char* page_data,size_t size, size_t nmemb,std::string *data);
        virtual std::string confirm_url(std::string ip);
        virtual void curl_http();
        virtual void parse_data();
        virtual void insert_db();
        std::string revise_result(std::string&);
        ~ipcn_spider_ip();
        int cidr(std::string ip_s, std::string ip_e, std::string des); 
    public:
        std::string buffer;
        char url[1024];
        char User_Agent[500];
        long code;
        std::string ip_begin;
        std::string ip_end;
        std::string current_ip;

        int fd;
        char unRet;
        int step;

        std::string current_result;
        std::string next_result;
        int ip_back;

        int once_flag;

        std::string write_current_data;
        std::string write_next_data;
        std::string write_last_ip;

        int once1_flag;

        std::string pre_ip;
        std::string pre_data;
};


