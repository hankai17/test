#include <sys/types.h>  
#include <netinet/in.h>  
#include <ctype.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <arpa/inet.h>  
#include <unistd.h>

/* 
 * Ascii internet address interpretation routine. 
 * The value returned is in network order. 
 */  
    unsigned int  
__inet_aton(const char *cp)  
{  
    unsigned int val = 0;  
    unsigned int tmp = 0;  
    char c;  
    int base = 10;  
    int shift = 32;  

    c = *cp;  
    for (;;) {//���Ը���һ�����ⳤ�ȵ��ַ�����  
        tmp = 0;  
        for (;;) {//���'.'�ָ��ÿһ���ַ�������ֵ��  
            if (isdigit(c)) {  
                tmp = (tmp * base) + (c - '0');  
                c = *++cp;  
            } else break;  
        }  

        shift -= 8;  
        tmp <<= shift;  
        val += tmp;   

        if (c == '.') {  
            c = *++cp;  
        } else  
            break;  
    }  

    /* 
     * Check for trailing characters. 
     */  
    if (c != '\0' && (!isspace(c)))  
        goto ret_0;  

    return (htonl (val));//���������ֽ���  
ret_0:  
    return (0);  
}  

/* The interface of this function is completely stupid, it requires a 
   static buffer.  We relax this a bit in that we allow one buffer for 
   each thread.  */  
static char buffer[18];//��Ϊ�⺯������ʽ����������������ԭ��  
    char *  
__inet_ntoa (unsigned int in)  
{  
    unsigned char *bytes = (unsigned char *) &in;  
    snprintf (buffer, sizeof (buffer), "%d.%d.%d.%d",  
            bytes[0], bytes[1], bytes[2], bytes[3]);//�����ֽ�����С�˱�ʾ����һ����byte[0]��  

    return buffer;  
}  

int main()  
{  
    //printf("ip :%lu\n",__inet_aton("192.168.1.105 "));    
    unsigned long ip = __inet_aton("192.168.1.105 ");
    while(1)
    {
        ip++;
        //printf("str:%s\n",__inet_ntoa(__inet_aton("192.168.1.105")));  
        printf("str:%s\n",__inet_ntoa(ip));  
        sleep(1);
    }
    return 0;     
}  
