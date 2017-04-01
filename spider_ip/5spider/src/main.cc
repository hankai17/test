#include"spider_ip.hh"
#include"tb.hh"
//#include"ipcn.hh"
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
    for(int j = 0; j < 100; j++)
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
    for(int j = 0; j < 100; j++)
    {
       std::thread t1(DoBussiness,new ip138_spider_ip(ip_b[j],ip_e[j])); 
       t1.join();
    }

    //std::thread t1(DoBussiness,new ip138_spider_ip("202.38.164.0","202.38.177.0")); 
    //std::thread t1(DoBussiness,new ip138_spider_ip("61.128.0.0","61.192.0.0")); 
    //std::thread t1(DoBussiness,new ip138_spider_ip("27.8.0.0","27.32.0.0")); 
    //std::thread t1(DoBussiness,new ip138_spider_ip("59.191.0.0","60.0.0.0")); 
    //t1.join();
    return;    
}

void test()
{
    //std::thread t1(tb);
    std::thread t2(ip138);

    //t1.join();
    t2.join();
    return;
}

int main()
{
    
    int ip[][8] = 
    { 
        { 1, 12, 0, 0, 1, 16, 0, 0 },  //1-1 . 12-15 . 0-255 . 0-255
        { 1, 24, 0, 0, 1, 32, 0, 0 },  //1-1 . 24-31 . 0-255 . 0-255
        { 27, 8, 0, 0, 27, 32, 0, 0 },
        { 27, 36, 0, 0, 27, 48, 0, 0 },
        { 27, 50, 128, 0, 27, 51, 0, 0 },
        { 27, 54, 192, 0, 27, 55, 0, 0 },
        { 27, 128, 0, 0, 27, 130, 0, 0 },
        { 27, 144, 0, 0, 27, 145, 0, 0 },
        { 27, 148, 0, 0, 27, 160, 0, 0 },
        { 27, 184, 0, 0, 27, 228, 0, 0 },
        { 58, 14, 0, 0, 58, 26, 0, 0 },
        { 58, 30, 0, 0, 58, 64, 0, 0 },
        { 58, 66, 0, 0, 58, 68, 0, 0 },
        { 58, 68, 128, 0, 58, 69, 0, 0 },
        { 58, 82, 0, 0, 58, 84, 0, 0 },
        { 58, 87, 64, 0, 58, 87, 128, 0 },
        { 58, 99, 128, 0, 58, 102, 0, 0 },
        { 58, 116, 0, 0, 58, 120, 0, 0 },
        { 58, 128, 0, 0, 58, 136, 0, 0 },
        { 58, 144, 0, 0, 58, 145, 0, 0 },
        { 58, 154, 0, 0, 58, 156, 0, 0 },
        { 58, 192, 0, 0, 58, 224, 0, 0 },
        { 58, 240, 0, 0, 59, 0, 0, 0 },
        { 59, 32, 0, 0, 59, 84, 0, 0 },
        { 59, 107, 0, 0, 59, 112, 0, 0 },
        { 59, 151, 0, 0, 59, 151, 128, 0 },
        { 59, 155, 0, 0, 59, 156, 0, 0 },
        { 59, 172, 0, 0, 59, 176, 0, 0 },
        { 59, 191, 0, 0, 60, 0, 0, 0 },
        { 60, 0, 0, 0, 60, 32, 0, 0 },
        { 60, 55, 0, 0, 60, 56, 0, 0 },
        { 60, 63, 0, 0, 60, 64, 0, 0 },
        { 60, 160, 0, 0, 60, 192, 0, 0 },
        { 60, 194, 0, 0, 60, 196, 0, 0 },
        { 60, 200, 0, 0, 60, 224, 0, 0 },
        { 60, 232, 0, 0, 60, 234, 0, 0 },
        { 60, 235, 0, 0, 60, 236, 0, 0 },
        { 60, 245, 128, 0, 60, 246, 0, 0 },
        { 60, 247, 0, 0, 60, 248, 0, 0 },
        { 60, 252, 0, 0, 60, 253, 0, 0 },
        { 60, 253, 128, 0, 60, 254, 0, 0 },
        { 60, 255, 0, 0, 61, 0, 0, 0},
        { 61, 4, 80, 0, 61, 4, 96, 0 },
        { 61, 4, 176, 0, 61, 4, 192, 0 },
        { 61, 8, 160, 0, 61, 8, 176, 0 },
        { 61, 28, 0, 0, 61, 28, 128, 0 },
        { 61, 29, 128, 0, 61, 30, 0, 0 },
        { 61, 45, 128, 0, 61, 45, 192, 0 },
        { 61, 47, 128, 0, 61, 47, 192, 0 },
        { 61, 48, 0, 0, 61, 56, 0, 0 },
        { 61, 87, 192, 0, 61, 88, 0, 0 },
        { 61, 128, 0, 0, 61, 192, 0, 0 },
        { 61, 232, 0, 0, 61, 238, 0, 0 },
        { 61, 240, 0, 0, 61, 244, 0, 0 },
        { 110, 6, 0, 0, 110, 8, 0, 0 },
        { 110, 16, 0, 0, 110, 20, 0, 0 },
        { 110, 40, 0, 0, 110, 44, 0, 0 },
        { 110, 48, 0, 0, 110, 49, 0, 0 },
        { 110, 51, 0, 0, 110, 54, 0, 0 },
        { 110, 56, 0, 0, 110, 66, 0, 0 },
        { 110, 72, 0, 0, 110, 74, 0, 0 },
        { 110, 75, 0, 0, 110, 76, 64, 0 },
        { 110, 76, 192, 0, 110, 77, 128, 0 },
        { 110, 80, 0, 0, 110, 92, 0, 0 },
        { 110, 94, 0, 0, 110, 128, 0, 0 },
        { 110, 152, 0, 0, 110, 158, 0, 0 },
        { 110, 166, 0, 0, 110, 168, 0, 0 },
        { 110, 172, 192, 0, 110, 173, 48, 0 },
        { 110, 173, 64, 0, 110, 173, 128, 0 },
        { 110, 173, 192, 0, 110, 173, 224, 0 },
        { 110, 176, 0, 0, 110, 224, 0, 0 },
        { 110, 228, 0, 0, 110, 232, 0, 0 },
        { 110, 232, 32, 0, 110, 232, 64, 0 },
        { 110, 236, 0, 0, 110, 238, 0, 0 },
        { 110, 240, 0, 0, 111, 0, 0, 0 },
        { 111, 0, 0, 0, 111, 64, 0, 0 },
        { 111, 66, 0, 0, 111, 67, 0, 0 },
        { 111, 67, 192, 0, 111, 67, 208, 0 },
        { 111, 68, 64, 0, 111, 68, 96, 0 },
        { 111, 72, 0, 0, 111, 80, 0, 0 },
        { 111, 85, 0, 0, 111, 86, 0, 0 },
        { 111, 91, 192, 0, 111, 91, 224, 0 },
        { 111, 112, 0, 0, 111, 118, 0, 0 },
        { 111, 119, 64, 0, 111, 119, 160, 0 },
        { 111, 120, 0, 0, 111, 125, 0, 0 },
        { 111, 126, 0, 0, 111, 168, 0, 0 },
        { 111, 170, 0, 0, 111, 171, 0, 0 },
        { 111, 172, 0, 0, 111, 184, 0, 0 },
        { 111, 186, 0, 0, 111, 188, 0, 0 },
        { 111, 192, 0, 0, 111, 216, 0, 0 },
        { 111, 221, 128, 0, 111, 223, 0, 0 },
        { 111, 223, 240, 0, 111, 223, 244, 0 },
        { 111, 223, 248, 0, 111, 223, 252, 0 },
        { 111, 224, 0, 0, 111, 232, 0, 0 },
        { 111, 235, 96, 0, 111, 235, 128, 0 },
        { 111, 235, 160, 0, 111, 235, 192, 0 },
        { 112, 0, 0, 0, 112, 68, 0, 0 },
        { 112, 73, 0, 0, 112, 76, 0, 0 },
        { 112, 80, 0, 0, 112, 88, 0, 0 },
        { 112, 109, 128, 0, 112, 110, 0, 0 } 
    };
    char str_ip_begin[16] = {0};
    char str_ip_end[16] = {0};

    for (int m = 0; m < 100; m++)
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

