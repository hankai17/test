#include"spider_ip.hh"

void spider_ip::make_spider()
{
    curl_http();
}

spider_ip::~spider_ip()
{
    std::cout<<"delet spider_ip()"<<std::endl; 
}

char* spider_ip::int_2_str(int ip, char *buf)  
{  
    sprintf(buf, "%u.%u.%u.%u",  
            (unsigned char )*((char *)&ip + 3),  
            (unsigned char )*((char *)&ip + 2),  
            (unsigned char )*((char *)&ip + 1),  
            (unsigned char )*((char *)&ip + 0));  
    //std::cout<<"buf="<<buf<<std::endl;
    return buf;  
}  

int spider_ip::enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,int outSize)  
{  
    assert(pOutput != NULL);  
    assert(outSize >= 6);  

    if ( unic <= 0x0000007F )  
    {  
        // * U-00000000 - U-0000007F:  0xxxxxxx  
        *pOutput     = (unic & 0x7F);  
        return 1;  
    }  
    else if ( unic >= 0x00000080 && unic <= 0x000007FF )  
    {  
        // * U-00000080 - U-000007FF:  110xxxxx 10xxxxxx  
        *(pOutput+1) = (unic & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 6) & 0x1F) | 0xC0;  
        return 2;  
    }  
    else if ( unic >= 0x00000800 && unic <= 0x0000FFFF )  
    {  
        // * U-00000800 - U-0000FFFF:  1110xxxx 10xxxxxx 10xxxxxx  
        *(pOutput+2) = (unic & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >>  6) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 12) & 0x0F) | 0xE0;  
        return 3;  
    }  
    else if ( unic >= 0x00010000 && unic <= 0x001FFFFF )  
    {  
        // * U-00010000 - U-001FFFFF:  11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+3) = (unic & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 12) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 18) & 0x07) | 0xF0;  
        return 4;  
    }  
    else if ( unic >= 0x00200000 && unic <= 0x03FFFFFF )  
    {  
        // * U-00200000 - U-03FFFFFF:  111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+4) = (unic & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 18) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 24) & 0x03) | 0xF8;  
        return 5;  
    }  
    else if ( unic >= 0x04000000 && unic <= 0x7FFFFFFF )  
    {  
        // * U-04000000 - U-7FFFFFFF:  1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx  
        *(pOutput+5) = (unic & 0x3F) | 0x80;  
        *(pOutput+4) = ((unic >>  6) & 0x3F) | 0x80;  
        *(pOutput+3) = ((unic >> 12) & 0x3F) | 0x80;  
        *(pOutput+2) = ((unic >> 18) & 0x3F) | 0x80;  
        *(pOutput+1) = ((unic >> 24) & 0x3F) | 0x80;  
        *pOutput     = ((unic >> 30) & 0x01) | 0xFC;  
        return 6;  
    }  

    return 0;  
}  

std::string spider_ip::change_2_utf8(std::string str)
{
    char* data = (char*)str.data();
    char* foo = strtok(data,"\\u");
    std::string str1;
    char tmp_str[128] = {0};
    while(foo != NULL)
    {
        std::string s = { foo };
        foo = strtok(NULL,"\\u");

        enc_unicode_to_utf8_one((unsigned long)(hex2dec((char*)s.data())), (unsigned char*)tmp_str,128);  
        std::string str_tmp = {tmp_str};
        str1 += str_tmp;	
    }
    return str1;
}

int spider_ip::c2i(char ch)  
{  
    if(isdigit(ch))  
        return ch - 48;  
    if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )  
        return -1;  
    if(isalpha(ch))  
        return isupper(ch) ? ch - 55 : ch - 87;  
    return -1;  
}  

int spider_ip::hex2dec(char *hex)  
{  
    int len;  
    int num = 0;  
    int temp;  
    int bits;  
    int i;  
    len = strlen(hex);  
    for (i=0, temp=0; i<len; i++, temp=0)  
    {  
        temp = c2i( *(hex + i) );  
        bits = (len - i - 1) * 4;  
        temp = temp << bits;  
        num = num | temp;  
    }  
    return num;  
}  
