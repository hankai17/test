#include<iostream>
#include<string.h>
#include<curl/curl.h>
#include <unistd.h>  

#define NO '0'
#define YES '1'
#define FILE_NAME "curl.html"
static std::string buffer;
/*
static size_t write_data(void *ptr, size_t size, size_t nmemb,void *stream)
{
    int len = size * nmemb;  
    int written = len;  
    FILE *fp = NULL;  
    if (access((char*) stream, 0) == -1) {  
        fp = fopen((char*) stream, "wb");  
    } else {  
        fp = fopen((char*) stream, "ab");  
    }  
    if (fp) {  
        fwrite(ptr, size, nmemb, fp);  
    }  
    // printf("%s\n",ptr);  
    fclose(fp);  
    return written;  
}  
*/

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

    memcpy(url, "ip.taobao.com/service/getIpInfo.php?ip=123.120.175.224", strlen("ip.taobao.com/service/getIpInfo.php?ip=123.120.175.224"));
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
    //取country
    //std::cout<<"pos="<<buf.find("country\":\"")<<std::endl;//19 is c
    //std::cout<<"pos="<<buf.find("\"",buf.find("country\":\"")+strlen("country\":\""))<<std::endl;//41
    //std::cout<<"pos="<<buf.find("country\":\"")+strlen("country\":\"")<<std::endl;//29 is '\'
    std::string country = buf.substr(buf.find("country\":\"")+strlen("country\":\""), buf.find("\"",buf.find("country\":\"")+strlen("country\":\""))-(buf.find("country\":\"")+strlen("country\":\"")));
    std::cout<<"country="<<country<<std::endl;
    std::string provice = buf.substr(buf.find("region\":\"")+strlen("region\":\""), buf.find("\"",buf.find("region\":\"")+strlen("region\":\""))-(buf.find("region\":\"")+strlen("region\":\"")));
    std::cout<<"provice="<<provice<<std::endl;
    std::string city = buf.substr(buf.find("city\":\"")+strlen("city\":\""), buf.find("\"",buf.find("city\":\"")+strlen("city\":\""))-(buf.find("city\":\"")+strlen("city\":\"")));
    std::cout<<"city="<<city<<std::endl;
    std::string isp = buf.substr(buf.find("isp\":\"")+strlen("isp\":\""), buf.find("\"",buf.find("isp\":\"")+strlen("isp\":\""))-(buf.find("isp\":\"")+strlen("isp\":\"")));
    std::cout<<"isp="<<isp<<std::endl;
    std::string ip = buf.substr(buf.find("ip\":\"")+strlen("ip\":\""), buf.find("\"",buf.find("ip\":\"")+strlen("ip\":\""))-(buf.find("ip\":\"")+strlen("ip\":\"")));
    std::cout<<"ip="<<ip<<std::endl;

}

int main()
{
    curl_ics();
    std::cout<<"buffer="<<buffer<<std::endl;
    parse(buffer);
    return 0;
}

