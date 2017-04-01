#pragma once
//#ifndef _SPIDER_IP_H_
//#define _SPIDER_IP_H_

#include<iostream>
#include<string.h>
#include<curl/curl.h>
#include<unistd.h>  
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<fcntl.h>
#include<stdlib.h>

#define NO '0'
#define YES '1'
//using size_t = unsigned int;

class spider_ip
{
    public:
        virtual std::string confirm_url(std::string) = 0; 
        virtual void curl_http() = 0;
        virtual void parse_data() = 0;
        virtual void insert_db() = 0;
        virtual ~spider_ip() = 0;

        void make_spider();
        char* int_2_str(int ip, char *buf);  
        int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,int outSize);  
		std::string change_2_utf8(std::string str);
		int c2i(char ch);  
		int hex2dec(char *hex);  
};
//#endif

