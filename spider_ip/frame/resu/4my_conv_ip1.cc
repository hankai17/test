#include <iostream>  
#include <cstring>  
#include <cstdio>  
using namespace std;  

//IP�ַ���ת32λint��   
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

//������IP��ַת�����ַ���IP��ַ   
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
    //���һ�������ַ���תint   
      while (cin>>test)  
      {  
          cout<<IPStrToInt(test)<<endl;  
      }  
*/
    //��ڶ�������intת�ַ���  
    while (cin>>ip)   
    {  
        cout<<IntToStr1(ip, test)<<endl;       
    }  

    return 0;  
}  
