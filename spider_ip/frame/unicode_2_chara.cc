#include<iostream>
#include<string.h>
#include<assert.h>

int enc_unicode_to_utf8_one(unsigned long unic, unsigned char *pOutput,int outSize)  
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
/*
int main()
{
    unsigned char buf[32] = {0};
    enc_unicode_to_utf8_one(0x7f8e, buf,32);  
    std::cout<<buf<<std::endl;
    return 0;
}
*/
std::wstring unicode2string(char * str)
{  
    std::wstring rst;  
    bool escape = false;  
    int len = strlen(str);  
    int intHex;  
    char tmp[5];  
    memset(tmp, 0, 5);  
    for (int i = 0; i < len; i++)  
    {  
        char c = str[i];  
        switch (c)  
        {  
            case '//':  
            case '%':  
                escape = true;  
                break;  
            case 'u':  
            case 'U':  
                if (escape)  
                {  
                    memcpy(tmp, str+i+1, 4);  
                    sscanf(tmp, "%x", &intHex); //把16进制字符转换为数字  
                    rst.push_back(intHex);  
                    i+=4;  
                    escape=false;  
                }else{  
                    rst.push_back(c);  
                }  
                break;  
            default:  
                rst.push_back(c);  
                break;  
        }  
    }  
    std::wcout<<rst<<std::endl;
    return rst;  
}  
int main()
{
    char para[] = "/u7f8e";
    std::wstring chara = unicode2string(para);
    std::wcout<<"chara="<<chara<<std::endl;
    return 0;
}

/*
   int main()
   {
   unsigned short unicode = 0x6c49;
   char chara[3] = {0};
   chara[0] = unsigned(unicode)/256;
   chara[1] = unsigned(unicode)%256;
   chara[2] = 0;
   std::cout<<"chara="<<chara<<std::endl;
   return 0;
   }
   */
   /*
int main()
{
    std::local::global(std::local(""));
    wchar_t wc = L'狗'; //wchar_t == unsigned short
    wchar_t wc1 = 31616;
    std::wcout<<wc<<" "<<wc1<<std::endl;
    return 0;
}

*/
