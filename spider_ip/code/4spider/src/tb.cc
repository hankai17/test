#include"tb.hh"
#include<iconv.h>
#include <chrono>
#include <thread>

tb_spider_ip::tb_spider_ip(std::string ip_b, std::string ip_e)
{
    step = 0;
    ip_back = 0;
    current_result = "";
    next_result = "";
    ip_begin = ip_b; 
    ip_end = ip_e; 
    current_ip = ip_begin;
    once_flag = 0;

    write_current_data = "";
    write_next_data = "";
    memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
            strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));
    memcpy(url, "ip.taobao.com/service/getIpInfo.php?ip=", 
            strlen("ip.taobao.com/service/getIpInfo.php?ip="));
    char file_name[64] = {0};
    sprintf(file_name,"%s~~%s_tb.txt",(char*)ip_b.data(),(char*)ip_e.data());
    std::cout<<"file_name="<<file_name<<std::endl;
    fd = open(file_name, O_RDWR|O_TRUNC|O_CREAT, 0664);
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

std::string tb_spider_ip::confirm_url(std::string ip) //È·¶¨ip·¶Î§ ×éurl´®
{
    if(once_flag == 0)
    {
        unsigned long ip_long = inet_network(ip.data());
        char tmp[32];
        int_2_str(ip_long,tmp); 
        std::string current_ip1 = {tmp};
        std::string new_url = url + current_ip1; 
        once_flag = 1;
        return new_url;
    }
    else
    {
        if(ip != ip_end)
        {
            unsigned long ip_long = inet_network(ip.data());
            if(ip_back)
            {
                ip_long = ip_long - step + 1;
                step = 1;
                ip_back = 0;
            }
            else
            {
                ip_long += step;
            }
            char tmp[32];
            int_2_str(ip_long,tmp); 
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
}

void tb_spider_ip::curl_http() 
{
    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, User_Agent);
    while(1)
    {
        std::chrono::milliseconds sleepDuration(500);
        std::this_thread::sleep_for(sleepDuration);
        std::string&& current_url = confirm_url(current_ip);
        std::cout<<"current_url="<<current_url<<std::endl;
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
                ip_back =  1;
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
        return;
    }
    std::string code = buffer.substr(buffer.find("code\":")+strlen("code\":"), buffer.find(",",buffer.find("code\":")+strlen("code\":"))-(buffer.find("code\":")+strlen("code\":")));
    if( code == "1")
    {
        std::cout<<"tb web error"<<std::endl;
        ip_back =  1;
        return;
    }
    country = buffer.substr(buffer.find("country\":\"")+strlen("country\":\""), buffer.find("\"",buffer.find("country\":\"")+strlen("country\":\""))-(buffer.find("country\":\"")+strlen("country\":\"")));
    province = buffer.substr(buffer.find("region\":\"")+strlen("region\":\""), buffer.find("\"",buffer.find("region\":\"")+strlen("region\":\""))-(buffer.find("region\":\"")+strlen("region\":\"")));
    city = buffer.substr(buffer.find("city\":\"")+strlen("city\":\""), buffer.find("\"",buffer.find("city\":\"")+strlen("city\":\""))-(buffer.find("city\":\"")+strlen("city\":\"")));
    isp = buffer.substr(buffer.find("isp\":\"")+strlen("isp\":\""), buffer.find("\"",buffer.find("isp\":\"")+strlen("isp\":\""))-(buffer.find("isp\":\"")+strlen("isp\":\"")));
    ip = buffer.substr(buffer.find("ip\":\"")+strlen("ip\":\""), buffer.find("\"",buffer.find("ip\":\"")+strlen("ip\":\""))-(buffer.find("ip\":\"")+strlen("ip\":\"")));

    /*
    std::string&& s1 = change_2_utf8(country);
    std::string&& s2 = change_2_utf8(province);
    std::string&& s3 = change_2_utf8(city);
    std::string&& s4 = change_2_utf8(isp);
    */
    std::string s1 = change_2_utf8(country);
    std::string s2 = change_2_utf8(province);
    std::string s3 = change_2_utf8(city);
    std::string s4 = change_2_utf8(isp);
    std::string detail = s1 + s2 + s3;
    std::string target = current_ip + "   " + detail  + "   " + s4 + "\n";
    std::cout<<"target="<<target<<std::endl;

    if(current_result == "" )
    {
        current_result = detail;
        write_current_data = detail;

        if(write_next_data != write_current_data)
        {
            write(fd, (char*)target.data(), strlen(target.data())); 
        }
        write_current_data = write_next_data;
        write_next_data = "";
        step = 1;
    }
    else 
    {
        next_result = detail; 
    }

    if( current_result != "" && next_result != "" )
    {
        if( !strcmp(current_result.data(),next_result.data())) 
        {
            step *= 2;
            write_next_data = detail;
            current_result = next_result;
            next_result = "";
        }
        else
        {
            current_result = "";
            next_result = "";
            ip_back = 1;
        }
    }
}

void tb_spider_ip::insert_db()
{
    //std::cout << "insert_db" << std::endl;
}

