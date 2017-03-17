#include <iconv.h>  

int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)  
{  
    iconv_t cd;  
    int rc;  
    char **pin = &inbuf;  
    char **pout = &outbuf;  

    cd = iconv_open(to_charset,from_charset);  
    if (cd==0)  
        return -1;  
    memset(outbuf,0,outlen);  
    if (iconv(cd,pin,&inlen,pout,&outlen) == -1)  
        return -1;  
    iconv_close(cd);  
    return 0;  
}  

int u2g(char *inbuf,int inlen,char *outbuf,int outlen)  
{  
    return code_convert("utf-8","gb2312",inbuf,inlen,outbuf,outlen);  
}  

int g2u(char *inbuf,size_t inlen/*becarefull*/,char *outbuf,size_t outlen)  
{  
    return code_convert("gb2312","utf-8",inbuf,inlen,outbuf,outlen);  
}  
