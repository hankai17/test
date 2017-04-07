#include<iostream>
#include<string.h>
#include<curl/curl.h>
#include <unistd.h>  
#include <iconv.h>  

#define NO '0'
#define YES '1'
#define FILE_NAME "curl.html"

static std::string buffer;
static int write_data(char* page_data,size_t size, size_t nmemb,std::string *data)
{
    if(data == NULL)
        return 0;
    data->append(page_data);
    return size * nmemb;
}
char curl_ics()
{
    char unRet = NO;
    long code;

    char url[1024] =  {0}; 
    char User_Agent[500] = {0};
    memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
            strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));

    memcpy(url, "www.ip138.com/ips138.asp?ip=123.135.131.37", strlen("www.ip138.com/ips138.asp?ip=182.135.131.37")); //cache-control no-cache
    //html格式
    CURL * curl = curl_easy_init(); //初始化

    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, User_Agent);

    if (curl != NULL)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(curl, CURLOPT_HEADER, 0); //一并获取响应头
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 0); //Get 请求
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
        //curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); //302
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); //receive data
        curl_easy_setopt(curl, CURLOPT_WRITEDATA,&buffer); // write_data param

        unRet = curl_easy_perform(curl);
        if (CURLE_OK == unRet)
        {
            unRet = YES;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
            if (code >= 400)
            {
                unRet = NO;
            }
            std::cout<<" response code: "<<code<<std::endl;
        }
        else
        {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
            std::cout<<" response code: "<<code<<std::endl;
            unRet = NO;
        }
        curl_easy_cleanup(curl);
    }
    return unRet;
}
int parse(std::string& buf)
{
    if(buf.size() == 0)
    {
        std::cout<<"no data!"<<std::endl;
        return 1;
    }
    std::cout<<"*****"<<std::endl;
    std::string detail = buf.substr(buf.find("本站数据：")+strlen("本站数据："), buf.find("<",buf.find("本站数据：")+strlen("本站数据："))-(buf.find("本站数据：")+strlen("本站数据：")));
    std::cout<<"detail="<<detail<<std::endl;
    


}

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

int g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)  
{  
    return code_convert((char*)"gb2312",(char*)"utf-8",inbuf,inlen,outbuf,outlen);  
}  

int main()
{
    curl_ics();
    parse(buffer);
    //std::cout<<"buffer="<<buffer<<std::endl;
    char out[1024*1024] = {0};
    g2u((char*)buffer.data(),(size_t)buffer.length(),out,(size_t)sizeof(out));
    //std::cout<<"out="<<out<<std::endl;
    std::string buf(out);
    std::cout<<"buf="<<buf<<std::endl;
    parse(buf);
    return 0;
}

