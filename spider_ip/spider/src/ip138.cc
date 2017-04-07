#include"ip138.hh"
#include<iconv.h>
#include <chrono>
#include <thread>
#include"log.hh"

int ip138_spider_ip::write_data(char* page_data,size_t size, size_t nmemb,std::string *data)
{
    if(data == NULL)
        return 0;
    data->append(page_data);
    return size * nmemb;
}

ip138_spider_ip::ip138_spider_ip(std::string ip_b, std::string ip_e)
{
    step = 0;
    ip_back = 0;
    current_result = "";
    next_result = "";
    ip_begin = ip_b; 
    ip_end = ip_e; 
    current_ip = ip_begin;
    once_flag = 0;
    once1_flag = 0;
    pre_ip = "";
    pre_data = "";

    write_current_data = "";
    write_next_data = "";
    write_last_ip = "";
    memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
            strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; \
        WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));
    memcpy(url, "www.ip138.com/ips138.asp?ip=", strlen("www.ip138.com/ips138.asp?ip=")); 
    char file_name[64] = {0};
    sprintf(file_name,"%s~~%s_ip138.txt",(char*)ip_b.data(),(char*)ip_e.data());
    //std::cout<<"file_name="<<file_name<<std::endl;
    fd = open(file_name, O_RDWR|O_TRUNC|O_CREAT, 0664);
}

ip138_spider_ip::~ip138_spider_ip()
{
    close(fd);
    //std::cout<<"~ip138_spider_ip()"<<std::endl; 
}

std::string ip138_spider_ip::confirm_url(std::string ip)
{
    if(once_flag == 0)
    {
        unsigned long ip_long = inet_network(ip.data());
        char tmp[32];
        int_2_str(ip_long,tmp); 
        std::string current_ip1 = {tmp};
        std::string new_url = url + current_ip1; 
        once_flag = 1;
        pre_ip = {ip};
        return new_url;
    }
    else
    {
        //if(ip != ip_end)
        if( inet_network(ip.data()) < inet_network(ip_end.data()) )
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
        else if( inet_network(ip.data()) > inet_network(ip_end.data()) )
        {
            unsigned long ip_long = inet_network(ip.data());
            ip_long = ip_long - step + 1;
            step = 1;
            ip_back = 0;
            char tmp[32];
            int_2_str(ip_long,tmp); 
            current_ip = {tmp};
            std::string new_url = url + current_ip; 
            return new_url;
        }
        else 
        {
            //std::cout<<"spider ip over"<<std::endl;
            std::string err_url = {"NULL"};
            return err_url;
        }
    }
    std::string err_url = {"NULL"};
    return err_url;
}

void ip138_spider_ip::curl_http()
{
    struct curl_slist *slist = NULL;
    slist = curl_slist_append(slist, User_Agent);
    while(1)
    {
        std::string&& current_url = confirm_url(current_ip);
        //std::cout<<"current_url="<<current_url<<std::endl;
        if(current_url == "NULL")
        {
            if(write_last_ip != ip_end)
            {
                //std::cout<<"This IP segment is something wrong"<<std::endl;
                Log(__FILE__, __LINE__,Level[2],0,"%s~~%s",(char*)(ip_begin.data()),(char*)(ip_end.data()));
            }
            break;
        }
        CURL* curl = curl_easy_init(); 
        std::chrono::milliseconds sleepDuration(500);
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
                    //std::cout<<"ok, response code: "<<code<<std::endl;
                }
            }
            else
            {
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
                //std::cout<<"no, response code: "<<code<<std::endl;
            }
            curl_easy_cleanup(curl);
        }
        else
        {
            //std::cout<<"curl is NULL!"<<std::endl;
        }

        parse_data();
        buffer = "";
        std::this_thread::sleep_for(sleepDuration);
    }
}

//确保传入的参数第一个字符非空格
std::string ip138_spider_ip::revise_result(std::string& str)
{
    if(str.data() == nullptr)
    {
        return "NULL";
    }
    int is_blank = 0;
    int str_num = 1;
    char* p = (char*)str.data();
    while(*p != '\0')
    {
        if(*p == ' ')
        {
            is_blank = 1;
            p++;
            continue;
        }
        else
        {
            p++; 
            if(is_blank)
            {
                str_num++; 
                is_blank = 0;
            }
        }
    }
    //std::cout<<"str_num="<<str_num<<std::endl;
    char data[1024] = {0};
    if(str_num == 1)
    {
        char* d1 = nullptr;
        char* tmp;
        strcpy(data,str.data());
        d1 = data;
        tmp = data;
        while(*tmp)
        {
            if(*tmp == ' ')
            {
                *tmp = '\0';
                tmp++;
            }
            else
            {
                tmp++;
            }
        }
        std::string res1 = {d1};
        std::string res = res1 + " " + "NULL"; 
        return res;
    }
    if(str_num == 2)
    {
        char* d1 = nullptr;
        char* d2 = nullptr;
        char* tmp;
        int is_blank = 0;
        strcpy(data,str.data());
        d1 = data;
        tmp = data;
        while(*tmp)
        {
            if(*tmp == ' ')
            {
                is_blank = 1;
                *tmp = '\0';
                tmp++;
                continue;
            }
            else
            {
                tmp++;
                if(is_blank)
                {
                    d2 = tmp - 1;
                    is_blank = 0;
                }
            }
        }
        std::string res1 = {d1};
        std::string res2 = {d2};
        std::string res3 = res1 + " " + res2;
        return res3;
    }
    if(str_num == 3)
    {
        char* d1 = nullptr;
        char* d2 = nullptr;
        char* d3 = nullptr;
        char* tmp;
        int is_blank = 0;
        int str_num = 1;
        int in_flag1 = 0;
        int in_flag2 = 0;
        strcpy(data,str.data());
        d1 = data;
        tmp = data;
        while(*tmp)
        {
            if(*tmp == ' ')
            {
                is_blank = 1;
                *tmp = '\0';
                tmp++;
                continue;
            }
            else
            {
                if(is_blank)
                {
                    //str_num++; 
                    ++str_num; 
                    is_blank = 0;
                }
                tmp++; 
            }
            if(str_num == 2 && in_flag1 == 0)
            {
                d2 = tmp - 1; 
                in_flag1 = 1;
            }
            if(str_num == 3 && in_flag2 == 0)
            {
                d3 = tmp - 1; 
                in_flag2 = 1;
            }
        }
        std::string s1 = {d1};
        std::string s2 = {d2};
        std::string s3 = {d3};

        std::string res = s1 + s2 + " " + s3;
        return res;
    }
    return "NULL";
}

int ip138_spider_ip::cidr(std::string ip_s, std::string ip_e, std::string des) 
{
    uint32_t ip_start, ip_end;
    struct in_addr addr;
    ip_start = ntohl(inet_addr((char*)ip_s.data()));
    ip_end = ntohl(inet_addr((char*)ip_e.data()));

    uint32_t net = ip_start;
    uint32_t mask;
    int i;
    while ( net <= ip_end ) 
    {
        for(i = 32; i >=0; i--) 
        {
            mask = (i == 0) ? 0 : 0xffffffff << (32 - i);
            if ( (net & mask) != net || (net | ~mask) > ip_end )
            {
                addr.s_addr = htonl(net);
                //std::cout<<"%s/"<<inet_ntoa(addr)<<"%d\n"<<i + 1;
                //std::cout<<inet_ntoa(addr)<<"/"<<i + 1<<std::endl;
                char tmp[128] = {0};
                sprintf(tmp,"%s/%d ",inet_ntoa(addr),i+1);
                std::string d = {tmp + des + "\n"};
                write(fd, (char*)d.data(), strlen(d.data())); 
                mask = 0xffffffff << (32 - i - 1);
                net = (net | ~mask) + 1;
                break;
            }
        }

        if( i == -1 ) 
        {
            //printf("0.0.0.0/0\n");
            //std::cout<<"0.0.0.0/0\n";
            break;
        }
        if( net == 0 ) 
        {
            break;
        }
    }
    return 0;
}

void ip138_spider_ip::parse_data()
{
    if(buffer.size() == 0)
    {
        //std::cout<<"no data!"<<std::endl;
        return;
    }
    char out[1024*1024];
    g2u((char*)buffer.data(),(size_t)buffer.length(),out,(size_t)sizeof(out));
    std::string buf(out);
    std::string detail1 = buf.substr(buf.find("本站数据：")+strlen("本站数据："), buf.find("<",buf.find("本站数据：")+strlen("本站数据："))-(buf.find("本站数据：")+strlen("本站数据："))); 
    //判断detail 
   //std::cout<<"detail1="<<detail1<<std::endl;
   std::string detail = revise_result(detail1); 
   //std::cout<<"test detail="<<detail<<std::endl;

   if(once1_flag == 0)
   {
        pre_data = detail;
        once1_flag = 1;
   }

    std::string des = current_ip + " " + detail + "\n";
    if(current_result == "" )
    {
        current_result = detail;
        write_current_data = detail;

        if(write_next_data != write_current_data)
        {
            //write(fd, (char*)des.data(), strlen(des.data())); 
            if(pre_ip != current_ip)
            {
                unsigned long ip_foo = inet_network(current_ip.data());
                ip_foo --;
                char tmp[32];
                int_2_str(ip_foo,tmp); 
                std::string current_ip_foo = {tmp};

                cidr(pre_ip,current_ip_foo,pre_data);
                pre_data = detail;
            } 

            pre_ip = current_ip;
            write_last_ip = current_ip;
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
            if(inet_network(current_ip.data()) <= inet_network(ip_end.data()))
            {
                step *= 2;
                write_next_data = detail;
                current_result = next_result;
                next_result = "";
            }
            else
            {
                current_ip = ip_end;
            }
        }
        else
        {
            current_result = ""; 
            next_result = "";
            ip_back = 1; 
        }
    }
}

void ip138_spider_ip::insert_db()
{
    //std::cout << "insert_db" << std::endl;
}

int ip138_spider_ip::code_convert(char *from_charset,char *to_charset,char *inbuf,size_t inlen,char *outbuf,size_t outlen)  
{  
    iconv_t cd;  
    char **pin = &inbuf;  
    char **pout = &outbuf;  

    cd = iconv_open(to_charset,from_charset);  
    if (cd==0)  
        return -1;  
    memset(outbuf,0,outlen);  
    if ((signed)iconv(cd,pin,&inlen,pout,&outlen) == -1)  
        return -1;  
    iconv_close(cd);  
    return 0;  
}  

int ip138_spider_ip::g2u(char *inbuf,size_t inlen,char *outbuf,size_t outlen)  
{  
    return code_convert((char*)"gb2312",(char*)"utf-8",inbuf,inlen,outbuf,outlen);  
}  

