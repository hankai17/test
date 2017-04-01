#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main() //整形转字符串
{
    char * ipaddr = NULL;
    char addr[20] ={0};
    in_addr inaddr;
    inaddr.s_addr=16777343;
    ipaddr = inet_ntoa(inaddr);
    strcpy(addr,ipaddr);  
    std::cout<<"addr="<<addr<<std::endl;
    return 0;
}

int main1(int aargc, char* argv[]) //整形lu 转字符串
{
    struct in_addr addr1,addr2;
    ulong l1,l2;
    l1= inet_addr("192.168.0.74");
    l2 = inet_addr("211.100.21.179");
    memcpy(&addr1, &l1, 4);
    memcpy(&addr2, &l2, 4);
    printf("%s : %s \n", inet_ntoa(addr1), inet_ntoa(addr2)); //注意这一句的运行结果
    printf("%s \n", inet_ntoa(addr1));
    printf("%s \n", inet_ntoa(addr2));
    return 0;
}

int main2() //字符串ip 转 整形lu
{
    //char str[]="255.255.255.255";
    char str[]="1.0.0.1";
    in_addr_t r1,r2,r3;
    struct in_addr inp;

    r1=inet_addr(str);
    if(r1==-1){
        printf("inet_addr return -1 when 255.255.255.255\n");
    }else{
        printf("inet_addr:ip=%lu\n",ntohl(r1));
    }

    r2=inet_network(str);
    if(r2==-1){
        printf("inet_network return -1 when 255.255.255.255\n");
    }else{
        printf("inet_network:ip=%lu\n",r2);
    }

    r3=inet_aton(str,&inp);
    if(r3==0){
        printf("inet_aton return -1 when 255.255.255.255\n");
    }else{
        printf("inet_aton:ip=%lu\n",ntohl(inp.s_addr));
    }

    return 0;
}
