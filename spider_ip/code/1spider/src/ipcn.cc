#include "ipcn.hh"

int ipcn_spider_ip::write_data(char* page_data,size_t size, size_t nmemb,std::string *data)
{
    if(data == NULL)
        return 0;
    data->append(page_data);
    return size * nmemb;
}

ipcn_spider_ip::ipcn_spider_ip(std::string ip_b, std::string ip_e)
{
    ip_begin = ip_b; 
    ip_end = ip_e; 
    current_ip = ip_begin;
    memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
            strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));
    memcpy(url, "http://www.ip.cn/ip-", strlen("http://www.ip.cn/ip-")); 
    fd = open("ipcn.txt", O_RDWR|O_TRUNC|O_CREAT, 0664);
}

ipcn_spider_ip::~ipcn_spider_ip()
{
   std::cout<<"~ipcn_spider_ip()"<<std::endl; 
}

std::string ipcn_spider_ip::confirm_url(std::string ip)
{
    if(ip != ip_end)
    {
        unsigned long ip_long = inet_network(ip.data());
        ip_long ++;
        char tmp[32];
        int_2_str(ip_long,tmp); 
        current_ip = {tmp};
        std::string new_url = url + current_ip + ".html"; 
        return new_url;
    }
    else
    {
        std::cout<<"spider ip over"<<std::endl;
        std::string err_url = {"NULL"};
        return err_url;
    }
}

void ipcn_spider_ip::curl_http()
{
    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, User_Agent);
    while(1)
    {
        std::string&& current_url = confirm_url(current_ip);
        //std::cout<<"current_url="<<current_url<<std::endl;
        if(current_url == "NULL")
            break;
        CURL* curl = curl_easy_init(); 
        if (curl != NULL)
        {
            curl_easy_setopt(curl, CURLOPT_URL, current_url.data());
            curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
            curl_easy_setopt(curl, CURLOPT_HEADER, 0); 
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 0);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data); 
            curl_easy_setopt(curl, CURLOPT_WRITEDATA,&buffer);

            unRet = curl_easy_perform(curl);
            if (CURLE_OK == unRet)
            {
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
                if (code >= 400)
                {
                    std::cout<<"ok, response code: "<<code<<std::endl;
                }
            }
            else
            {
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
                std::cout<<"no, response code: "<<code<<std::endl;
            }
            curl_easy_cleanup(curl);
        }
        else
        {
            std::cout<<"curl is NULL!"<<std::endl;
        }

        parse_data();
        buffer = "";
    }
}

void ipcn_spider_ip::parse_data()
{
    if(buffer.size() == 0)
    {
        std::cout<<"no data!"<<std::endl;
        return;
    }
    std::string detail = buffer.substr(buffer.find("所在地理位置：<code>")+strlen("所在地理位置：<code>"), buffer.find("<",buffer.find("所在地理位置：<code>")+strlen("所在地理位置：<code>"))-(buffer.find("所在地理位置：<code>")+strlen("所在地理位置：<code>")));

    //std::string city = buffer.substr(buffer.find("GeoIP: ")+strlen("GeoIP: "), buffer.find(",",buffer.find("GeoIP: ")+strlen("GeoIP: "))-(buffer.find("GeoIP: ")+strlen("GeoIP: ")));
    std::string des = current_ip + "   " + detail + "\n";
    write(fd, (char*)des.data(), strlen(des.data()));
}

void ipcn_spider_ip::insert_db()
{
    //std::cout << "insert_db" << std::endl;
}

