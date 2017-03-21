#include <iostream>  
#include <cstring>  
#include <cstdio>  
using namespace std;  

//IP字符串转32位int数   
unsigned int IPStrToInt(const char *ip)  
{  
    unsigned uResult = 0;  
    int nShift = 24;  
    int temp = 0;  
    const char *pStart = ip;  
    const char *pEnd = ip;  

    while (*pEnd != '\0')  
    {  
        while (*pEnd!='.' && *pEnd!='\0')  
        {  
            pEnd++;  
        }  
        temp = 0;  
        for (pStart; pStart!=pEnd; ++pStart)  
        {  
            temp = temp * 10 + *pStart - '0';  
        }     

        uResult += temp<<nShift;  
        nShift -= 8;  

        if (*pEnd == '\0')  
            break;  
        pStart = pEnd + 1;  
        pEnd++;  
    }  

    return uResult;  
}   

//将整数IP地址转换成字符串IP地址   
char *IntToStr(const int ip, char *buf)  
{  
    sprintf(buf, "%u.%u.%u.%u",  
            (unsigned char )*((char *)&ip + 0),  
            (unsigned char )*((char *)&ip + 1),  
            (unsigned char )*((char *)&ip + 2),  
            (unsigned char )*((char *)&ip + 3));  
    return buf;  
}  

char *IntToStr1(const int ip, char *buf)  
{  
    sprintf(buf, "%u.%u.%u.%u",  
            (unsigned char )*((char *)&ip + 3),  
            (unsigned char )*((char *)&ip + 2),  
            (unsigned char )*((char *)&ip + 1),  
            (unsigned char )*((char *)&ip + 0));  
    return buf;  
}  
int main()  
{  
    char test[16];  
    int ip;  
    /*
    //测第一个函数字符串转int   
      while (cin>>test)  
      {  
          cout<<IPStrToInt(test)<<endl;  
      }  
*/
    //测第二个函数int转字符串  
    while (cin>>ip)   
    {  
        cout<<IntToStr1(ip, test)<<endl;       
    }  

    return 0;  
}  
