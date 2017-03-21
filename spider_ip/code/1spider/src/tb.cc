#include"tb.hh"

tb_spider_ip::tb_spider_ip(std::string ip_b, std::string ip_e)
{
    ip_begin = ip_b; 
    ip_end = ip_e; 
    current_ip = ip_begin;
    memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
            strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));
    memcpy(url, "ip.taobao.com/service/getIpInfo.php?ip=", 
            strlen("ip.taobao.com/service/getIpInfo.php?ip="));
    fd = open("tb.txt", O_RDWR|O_TRUNC|O_CREAT, 0664);
}

tb_spider_ip::~tb_spider_ip()
{
    std::cout<<"~tb_spider_ip()"<<std::endl;
}

int tb_spider_ip::write_data(char* page_data,size_t size, size_t nmemb,std::string *data)
{
    if(data == NULL)
        return 0;
    data->append(page_data);
    return size * nmemb;
}

std::string tb_spider_ip::confirm_url(std::string ip) //确定ip范围 组url串
{
    if(ip != ip_end)
    {
        //转整形
        unsigned long ip_long = inet_network(ip.data());
        //++
        ip_long ++;
        //ip_long += 256*256;
        //转字符串
        char tmp[32];
        int_2_str(ip_long,tmp); 
        //组url 返回
        current_ip = {tmp};
        std::string new_url = url + current_ip; 
        return new_url;
    }
    else
    {
        std::cout<<"spider ip over"<<std::endl;
        std::string err_url = {"NULL"};
        return err_url;
    }

}

void tb_spider_ip::curl_http() 
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

void tb_spider_ip::parse_data()
{
    if(buffer.size() == 0)
    {
        //std::cout<<"-------------->no data!"<<std::endl;
        return;
    }
    std::string code = buffer.substr(buffer.find("code\":")+strlen("code\":"), buffer.find(",",buffer.find("code\":")+strlen("code\":"))-(buffer.find("code\":")+strlen("code\":")));
    //std::cout<<"tb code="<<code<<std::endl;
    if( code == "0")
    {
        //std::cout<<"success result!!!"<<std::endl;
    }
    else
    {
        //std::cout<<"failed result"<<std::endl;
        return;
    }
    country = buffer.substr(buffer.find("country\":\"")+strlen("country\":\""), buffer.find("\"",buffer.find("country\":\"")+strlen("country\":\""))-(buffer.find("country\":\"")+strlen("country\":\"")));
    province = buffer.substr(buffer.find("region\":\"")+strlen("region\":\""), buffer.find("\"",buffer.find("region\":\"")+strlen("region\":\""))-(buffer.find("region\":\"")+strlen("region\":\"")));
    city = buffer.substr(buffer.find("city\":\"")+strlen("city\":\""), buffer.find("\"",buffer.find("city\":\"")+strlen("city\":\""))-(buffer.find("city\":\"")+strlen("city\":\"")));
    isp = buffer.substr(buffer.find("isp\":\"")+strlen("isp\":\""), buffer.find("\"",buffer.find("isp\":\"")+strlen("isp\":\""))-(buffer.find("isp\":\"")+strlen("isp\":\"")));
    ip = buffer.substr(buffer.find("ip\":\"")+strlen("ip\":\""), buffer.find("\"",buffer.find("ip\":\"")+strlen("ip\":\""))-(buffer.find("ip\":\"")+strlen("ip\":\"")));

    std::string&& s1 = change_2_utf8(country);
    std::string&& s2 = change_2_utf8(province);
    std::string&& s3 = change_2_utf8(city);
    std::string&& s4 = change_2_utf8(isp);

    std::string target = current_ip + "   " + s1 + s2 + s3 + "   " + s4 + "\n";
    //std::cout<<"target="<<target;
    write(fd, (char*)target.data(), strlen(target.data()));
}

void tb_spider_ip::insert_db()
{
    //std::cout << "insert_db" << std::endl;
}

