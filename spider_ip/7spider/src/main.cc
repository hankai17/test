#include"spider_ip.hh"
#include"tb.hh"
#include"ipcn.hh"
#include"ip138.hh"
#include<thread>
#include<vector>
#include<chrono>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<functional>

#define INIT_DAEMON \
{ \
    if(fork() >0) exit(0); \
    setsid(); \
    if(fork() > 0) exit(0); \
}

std::vector<std::string> ip_b;
std::vector<std::string> ip_e;

std::vector< std::function<void(spider_ip*)> > vec_fun_tb;
std::vector< std::function<void(spider_ip*)> > vec_fun_ip138;

void DoBussiness(spider_ip* spider)
{
    spider->make_spider();
    delete spider;
}

void tb()
{
    for(int j = 0; j < 209; j++)
    {
        std::thread t1(DoBussiness,new tb_spider_ip(ip_b[j],ip_e[j])); 
        t1.join();
    }
    //std::thread t1(DoBussiness,new ip138_spider_ip("1.15.253.0","1.15.255.255")); 
    //std::thread t1(DoBussiness,new tb_spider_ip("1.31.100.0","1.32.0.0")); 
    //t1.join();
    return;    
}

void ip138()
{
    for(int j = 0; j < 209; j++)
    {
        std::thread t1(DoBussiness,new ip138_spider_ip(ip_b[j],ip_e[j])); 
        t1.join();
    }

    //std::thread t1(DoBussiness,new ip138_spider_ip("1.15.253.0","1.15.255.255")); 
    //std::thread t1(DoBussiness,new ip138_spider_ip("1.31.100.0","1.32.0.0")); 
    //t1.join();
    return;    
}

void ipcn()
{
    for(int j = 0; j < 209; j++)
    {
        std::thread t1(DoBussiness,new ipcn_spider_ip(ip_b[j],ip_e[j]));
        t1.join();
    }
    //std::thread t1(DoBussiness,new ipcn_spider_ip("202.38.164.0","202.38.177.0")); 
    //t1.join();
    return;
}

void test()
{
    std::thread t1(tb);
    std::thread t2(ip138);
    std::thread t3(ipcn);

    t1.join();
    t2.join();
    t3.join();
    return;
}

int main()
{

    int ip[][8] = 
    { 
        { 202,0,110,0 , 202,0,111,0 },
        { 202,0,160,0 , 202,0,184,0 },
        { 202,4,128,0 , 202,4,160,0 },
        { 202,4,252,0 , 202,5,0,0 },
        { 202,8,128,0 , 202,8,160,0 },
        { 202,10,64,0 , 202,10,80,0 },
        { 202,14,88,0 , 202,14,89,0 },
        { 202,14,235,0 , 202,14,239,0 },
        { 202,20,120,0 , 202,20,121,0 },
        { 202,22,248,0 , 202,23,0,0 },
        { 202,38,0,0 , 202,38,16,0 },
        { 202,38,64,0 , 202,38,139,0 },
        { 202,38,140,0 , 202,38,157,0 },
        { 202,38,158,0 , 202,38,162,0 },
        { 202,38,164,0 , 202,38,177,0 },
        { 202,38,184,0 , 202,39,0,0 },
        { 202,41,152,0 , 202,41,160,0 },
        { 202,41,240,0 , 202,42,0,0 },
        { 202,43,76,0 , 202,43,80,0 },
        { 202,46,32,0 , 202,46,64,0 },
        { 202,46,224,0 , 202,46,240,0 },
        { 202,60,112,0 , 202,60,128,0 },
        { 202,69,4,0 , 202,69,8,0 },
        { 202,69,16,0 , 202,69,32,0 },
        { 202,70,0,0 , 202,70,32,0 },
        { 202,74,8,0 , 202,74,16,0 },
        { 202,75,208,0 , 202,75,224,0 },
        { 202,85,208,0 , 202,85,224,0 },
        { 202,90,0,0 , 202,90,4,0 },
        { 202,90,224,0 , 202,90,240,0 },
        { 202,90,252,0 , 202,91,4,0 },
        { 202,91,128,0 , 202,91,132,0 },
        { 202,91,176,0 , 202,91,192,0 },
        { 202,91,224,0 , 202,92,4,0 },
        { 202,92,252,0 , 202,93,4,0 },
        { 202,93,252,0 , 202,94,32,0 },
        { 202,95,0,0 , 202,95,32,0 },
        { 202,95,252,0 , 202,122,40,0 },
        { 202,122,64,0 , 202,122,96,0 },
        { 202,122,112,0 , 202,122,120,0 },
        { 202,122,128,0 , 202,122,129,0 },
        { 202,123,96,0 , 202,123,112,0 },
        { 202,124,24,0 , 202,124,32,0 },
        { 202,125,176,0 , 202,125,192,0 },
        { 202,127,0,0 , 202,127,8,0 },
        { 202,127,12,0 , 202,127,32,0 },
        { 202,127,40,0 , 202,127,64,0 },
        { 202,127,112,0 , 202,128,0,0 },
        { 202,130,0,0 , 202,130,32,0 },
        { 202,130,224,0 , 202,131,0,0 },
        { 202,131,16,0 , 202,131,24,0 },
        { 202,131,48,0 , 202,131,64,0 },
        { 202,131,208,0 , 202,131,224,0 },
        { 202,136,48,0 , 202,136,64,0 },
        { 202,136,208,0 , 202,136,240,0 },
        { 202,141,160,0 , 202,141,192,0 },
        { 202,142,16,0 , 202,142,32,0 },
        { 202,143,16,0 , 202,143,32,0 },
        { 202,148,96,0 , 202,148,128,0 },
        { 202,149,160,0 , 202,149,176,0 },
        { 202,149,224,0 , 202,150,0,0 },
        { 202,150,16,0 , 202,150,32,0 },
        { 202,152,176,0 , 202,152,192,0 },
        { 202,153,48,0 , 202,153,64,0 },
        { 202,158,160,0 , 202,158,192,0 },
        { 202,160,176,0 , 202,160,192,0 },
        { 202,164,0,0 , 202,164,16,0 },
        { 202,164,25,0 , 202,164,26,0 },
        { 202,165,96,0 , 202,165,104,0 },
        { 202,165,176,0 , 202,165,192,0 },
        { 202,165,208,0 , 202,165,224,0 },
        { 202,168,160,0 , 202,168,192,0 },
        { 202,170,128,0 , 202,170,160,0 },
        { 202,170,216,0 , 202,170,224,0 },
        { 202,173,8,0 , 202,173,16,0 },
        { 202,173,224,0 , 202,174,0,0 },
        { 202,179,240,0 , 202,180,0,0 },
        { 202,180,128,0 , 202,180,160,0 },
        { 202,181,112,0 , 202,181,128,0 },
        { 202,189,80,0 , 202,189,96,0 },
        { 202,192,0,0 , 202,208,0,0 },
        { 203,18,50,0 , 203,18,51,0 },
        { 203,79,0,0 , 203,79,16,0 },
        { 203,80,144,0 , 203,80,160,0 },
        { 203,81,16,0 , 203,81,32,0 },
        { 203,83,56,0 , 203,83,64,0 },
        { 203,86,0,0 , 203,86,96,0 },
        { 203,88,0,0 , 203,88,4,0 },
        { 203,88,32,0 , 203,88,64,0 },
        { 203,88,192,0 , 203,88,224,0 },
        { 203,89,0,0 , 203,89,4,0 },
        { 203,90,0,0 , 203,90,4,0 },
        { 203,90,128,0 , 203,90,224,0 },
        { 203,91,32,0 , 203,91,64,0 },
        { 203,91,96,0 , 203,91,112,0 },
        { 203,91,120,0 , 203,91,128,0 },
        { 203,92,0,0 , 203,92,4,0 },
        { 203,92,160,0 , 203,92,192,0 },
        { 203,93,0,0 , 203,94,32,0 },
        { 203,95,0,0 , 203,95,8,0 },

        { 203,95,96,0 , 203,95,128,0 },
        { 203,99,16,0 , 203,99,32,0 },
        { 203,99,80,0 , 203,99,96,0 },
        { 203,100,32,0 , 203,100,48,0 },
        { 203,100,80,0 , 203,100,128,0 },
        { 203,100,192,0 , 203,100,208,0 },
        { 203,110,160,0 , 203,110,192,0 },
        { 203,114,244,0 , 203,114,248,0 },
        { 203,118,192,0 , 203,118,224,0 },
        { 203,118,248,0 , 203,118,252,0 },
        { 203,119,24,0 , 203,119,36,0 },
        { 203,119,80,0 , 203,119,84,0 },
        { 203,128,32,0 , 203,128,64,0 },
        { 203,128,96,0 , 203,128,160,0 },
        { 203,130,32,0 , 203,130,64,0 },
        { 203,132,32,0 , 203,132,64,0 },
        { 203,134,240,0 , 203,134,248,0 },
        { 203,135,96,0 , 203,135,128,0 },
        { 203,135,160,0 , 203,135,176,0 },
        { 203,142,219,0 , 203,142,220,0 },
        { 203,148,0,0 , 203,148,64,0 },
        { 203,152,64,0 , 203,152,96,0 },
        { 203,156,192,0 , 203,157,0,0 },
        { 203,158,16,0 , 203,158,24,0 },
        { 203,161,180,0 , 203,161,181,0 },
        { 203,161,192,0 , 203,161,224,0 },
        { 203,166,160,0 , 203,166,192,0 },
        { 203,171,224,0 , 203,171,240,0 },
        { 203,174,7,0 , 203,174,8,0 },
        { 203,174,96,0 , 203,174,128,0 },
        { 203,175,128,0 , 203,175,160,0 },
        { 203,175,192,0 , 203,176,0,0 },
        { 203,176,168,0 , 203,176,176,0 },
        { 203,184,80,0 , 203,184,96,0 },
        { 203,187,160,0 , 203,187,192,0 },
        { 203,190,96,0 , 203,190,112,0 },
        { 203,191,16,0 , 203,191,32,0 },
        { 203,191,64,0 , 203,191,128,0 },
        { 203,191,144,0 , 203,191,160,0 },
        { 203,192,0,0 , 203,192,32,0 },
        { 203,196,0,0 , 203,196,4,0 },
        { 203,207,64,0 , 203,208,20,0 },
        { 203,208,32,0 , 203,208,64,0 },
        { 203,209,224,0 , 203,210,0,0 },
        { 203,212,0,0 , 203,212,16,0 },
        { 203,212,80,0 , 203,212,96,0 },
        { 203,222,192,0 , 203,222,208,0 },
        { 203,223,0,0 , 203,223,16,0 },
        { 210,2,0,0 , 210,2,32,0 },
        { 210,5,0,0 , 210,5,48,0 },
        { 210,5,144,0 , 210,5,160,0 },
        { 210,12,0,0 , 210,14,0,0 },
        { 210,14,64,0 , 210,14,96,0 },
        { 210,14,112,0 , 210,15,192,0 },
        { 210,16,128,0 , 210,16,192,0 },
        { 210,21,0,0 , 210,23,0,0 },
        { 210,23,32,0 , 210,23,64,0 },
        { 210,25,0,0 , 210,48,0,0 },
        { 210,51,0,0 , 210,54,0,0 },
        { 210,56,192,0 , 210,56,224,0 },
        { 210,72,0,0 , 210,79,0,0 },
        { 210,79,64,0 , 210,79,128,0 },
        { 210,79,224,0 , 210,80,0,0 },
        { 210,82,0,0 , 210,84,0,0 },
        { 210,87,128,0 , 210,87,192,0 },
        { 210,185,192,0 , 210,186,0,0 },
        { 210,192,96,0 , 210,192,128,0 },
        { 211,64,0,0 , 211,72,0,0 },
        { 211,80,0,0 , 211,104,0,0 },
        { 211,136,0,0 , 211,168,0,0 },
        { 218,0,0,0 , 218,32,0,0 },
        { 218,56,0,0 , 218,100,0,0 },
        { 218,104,0,0 , 218,110,0,0 },
        { 218,185,192,0 , 218,185,224,0 },
        { 218,192,0,0 , 218,208,0,0 },
        { 218,240,0,0 , 218,248,0,0 },
        { 218,249,0,0 , 218,250,0,0 },
        { 219,72,0,0 , 219,73,0,0 },
        { 219,82,0,0 , 219,83,0,0 },
        { 219,128,0,0 , 219,160,0,0 },
        { 219,216,0,0 , 219,240,0,0 },
        { 219,242,0,0 , 219,248,0,0 },
        { 220,101,192,0 , 220,102,0,0 },
        { 220,112,0,0 , 220,116,0,0 },
        { 220,152,128,0 , 220,153,0,0 },
        { 220,154,0,0 , 220,156,0,0 },
        { 220,160,0,0 , 220,208,0,0 },
        { 220,231,0,0 , 220,231,64,0 },
        { 220,231,128,0 , 220,232,0,0 },
        { 220,232,64,0 , 220,232,128,0 },
        { 220,234,0,0 , 220,235,0,0 },
        { 220,242,0,0 , 220,244,0,0 },
        { 220,248,0,0 , 220,252,0,0 },
        { 221,0,0,0 , 221,12,192,0 },
        { 221,13,0,0 , 221,16,0,0 },
        { 221,122,0,0 , 221,124,0,0 },
        { 221,129,0,0 , 221,132,0,0 },
        { 221,133,224,0 , 221,134,0,0 },
        { 221,136,0,0 , 221,138,0,0 },
        { 221,172,0,0 , 221,184,0,0 },
        { 221,192,0,0 , 221,199,208,0 },
        { 221,199,224,0 , 221,240,0,0 },
        { 222,16,0,0 , 222,96,0,0 },
        { 222,125,0,0 , 222,126,0,0 },
        { 222,126,128,0 , 222,127,0,0 },
        { 222,128,0,0 , 222,144,0,0 },
        { 222,160,0,0 , 222,164,0,0 },
        { 222,168,0,0 , 222,224,0,0 },
        { 222,240,0,0 , 222,249,250,0 }
    };
    char str_ip_begin[16] = {0};
    char str_ip_end[16] = {0};
    for (int m = 0; m < (int)(sizeof(ip)/sizeof(ip[0])); m++)
    {
        sprintf( str_ip_begin,"%d.%d.%d.%d",ip[m][0],ip[m][1],ip[m][2],ip[m][3]);
        sprintf( str_ip_end,"%d.%d.%d.%d",ip[m][4],ip[m][5],ip[m][6],ip[m][7]);

        std::string ip_begin = str_ip_begin;
        std::string ip_end = str_ip_end;

        ip_b.push_back(ip_begin);
        ip_e.push_back(ip_end);

        memset(str_ip_begin,0,16);
        memset(str_ip_end,0,16);
    } 
    std::cout<<"ip_b.size="<<ip_b.size()<<std::endl;
    std::cout<<"ip_e.size="<<ip_e.size()<<std::endl;
    if(ip_b.size() != ip_e.size())
    {
        std::cout<<"parse ip group err"<<std::endl;
        return 0;
    }
    INIT_DAEMON
        test();
    return 0;
}

