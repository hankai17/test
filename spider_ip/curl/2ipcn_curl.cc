#include<iostream>
#include<string.h>
#include<curl/curl.h>
#include <unistd.h>  

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

    memcpy(url, "http://www.ip.cn/ip-1.0.15.254.html", strlen("http://www.ip.cn/ip-1.0.15.254.html")); 
    //结果是html 格式

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
    //std::string country = buf.substr(buf.find("country\":\"")+strlen("country\":\""), buf.find("\"",buf.find("country\":\"")+strlen("country\":\""))-(buf.find("country\":\"")+strlen("country\":\"")));
    //std::cout<<"pos="<<buf.find("所在地理位置: <code>")<<std::endl;//19 is c
    //std::cout<<"pos="<<buf.find("<code>")<<std::endl;//1842 is c
    //std::cout<<"pos="<<buf.find("</code>")<<std::endl;//1862 is c
    std::cout<<"*****"<<std::endl;
    std::string detail = buf.substr(buf.find("所在地理位置：<code>")+strlen("所在地理位置：<code>"), buf.find("<",buf.find("所在地理位置：<code>")+strlen("所在地理位置：<code>"))-(buf.find("所在地理位置：<code>")+strlen("所在地理位置：<code>")));
    std::cout<<"detail="<<detail<<std::endl;

    std::string city = buf.substr(buf.find("GeoIP: ")+strlen("GeoIP: "), buf.find(",",buf.find("GeoIP: ")+strlen("GeoIP: "))-(buf.find("GeoIP: ")+strlen("GeoIP: ")));
    std::cout<<"city="<<city<<std::endl;

    //std::cout<<"first , pos="<<buf.find(",",buf.find("GeoIP: "))<<std::endl;
    //std::cout<<"second , pos="<<buf.find(",",buf.find(",",buf.find("GeoIP: "))+1)<<std::endl;
    //std::cout<<"< pos="<<buf.find("<",buf.find(",",buf.find("GeoIP: "))+1)<<std::endl;
    /*
    std::string province = buf.substr(buf.find(",",buf.find("GeoIP: "))+2,buf.find(",",buf.find(",",buf.find("GeoIP: "))+1)- buf.find(",",buf.find("GeoIP: "))-2);
    std::cout<<"province="<<province<<std::endl;
    std::string country = buf.substr(buf.find(",",buf.find(",",buf.find("GeoIP: "))+1)+2 ,buf.find("<",buf.find(",",buf.find("GeoIP: "))+1)-(buf.find(",",buf.find(",",buf.find("GeoIP: "))+1)+2));
    std::cout<<"country="<<country<<std::endl;
    */

}
int main()
{
    curl_ics();
    std::cout<<"buffer="<<buffer<<std::endl;
    parse(buffer);
    return 0;
}

