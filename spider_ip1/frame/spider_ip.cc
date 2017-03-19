#include<iostream>
#include<string.h>
#include<curl/curl.h>
#include <unistd.h>  
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<assert.h>
#include <fcntl.h>
#include <stdlib.h>

#define NO '0'
#define YES '1'

class spider_ip
{
	public:
		virtual std::string confirm_url(std::string) = 0; 
		virtual void curl_http() = 0;
		virtual void parse_data() = 0;
		virtual void insert_db() = 0;
		void make_spider()
		{
			curl_http();
		}
		char *int_2_str(int ip, char *buf)  
		{  
			sprintf(buf, "%u.%u.%u.%u",  
					(unsigned char )*((char *)&ip + 3),  
					(unsigned char )*((char *)&ip + 2),  
					(unsigned char )*((char *)&ip + 1),  
					(unsigned char )*((char *)&ip + 0));  
			//std::cout<<"buf="<<buf<<std::endl;
			return buf;  
		}  
		int c2i(char ch)  
		{  
			if(isdigit(ch))  
				return ch - 48;  
			if( ch < 'A' || (ch > 'F' && ch < 'a') || ch > 'z' )  
				return -1;  
			if(isalpha(ch))  
				return isupper(ch) ? ch - 55 : ch - 87;  
			return -1;  
		}  

		int hex2dec(char *hex)  
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

};

class tb_spider_ip: public spider_ip
{
	public:
		tb_spider_ip(std::string ip_b, std::string ip_e)
		{
			ip_begin = ip_b; 
			ip_end = ip_e; 
			current_ip = ip_begin;
			char unRet = NO;
			memcpy(User_Agent,"User-Agent: Mozilla/5.0 (Windows NT 6.1; \
				WOW64; rv:52.0) Gecko/20100101 Firefox/52.0", 
					strlen("User-Agent: Mozilla/5.0 (Windows NT 6.1; \
				WOW64; rv:52.0) Gecko/20100101 Firefox/52.0"));
			memcpy(url, "ip.taobao.com/service/getIpInfo.php?ip=", 
					strlen("ip.taobao.com/service/getIpInfo.php?ip="));
			
    			fd = open("test.txt", O_RDWR|O_TRUNC|O_CREAT, 0664);
		}
	public:
		static int write_data(char* page_data,size_t size, size_t nmemb,std::string *data)
		{
			if(data == NULL)
				return 0;
			data->append(page_data);
			return size * nmemb;
		}

		virtual std::string confirm_url(std::string ip) //确定ip范围 组url串
		{
			//std::cout<<"current_ip="<<current_ip<<std::endl;
			if(ip != ip_end)
			{
				//转整形
				unsigned long ip_long = inet_network(ip.data());
				//++
				ip_long ++;
				//转字符串
				char tmp[32];
				int_2_str(ip_long,tmp); 
				//组url 返回
				current_ip = {tmp};
				//std::cout<<"current="<<current_ip<<std::endl;
				std::string new_url = url + current_ip; 
				std::cout<<"new_url="<<new_url<<std::endl;
				return new_url;
			}
			else
			{
				std::cout<<"spider ip over"<<std::endl;
				std::string err_url = {"NULL"};
				return err_url;
			}

		}

		virtual void curl_http() //内容存入buffer
		{
			struct curl_slist *slist = NULL;
			slist = curl_slist_append(slist, User_Agent);
			while(1)
			{
				std::cout<<"-------------------------------------------"<<std::endl;
				//组url
				//std::cout<<"current_ip="<<current_ip<<std::endl;
				std::string current_url = confirm_url(current_ip);
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
						unRet = YES;
						curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
						if (code >= 400)
						{
							unRet = NO;
						}
						std::cout<<"response code: "<<code<<std::endl;
					}
					else
					{
						curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
						std::cout<<"response code: "<<code<<std::endl;
						unRet = NO;
					}
					curl_easy_cleanup(curl);
				}
				else
				{
					std::cout<<"curl is NULL!"<<std::endl;
				}

				parse_data();
				//insert_db();
				//sleep(1);
				//清url
			}
		}

		std::string change_2_utf8(std::string str)
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
			std::cout<<"str1="<<str1<<std::endl;
			return str1;
		}

		virtual void parse_data()
		{
			if(buffer.size() == 0)
			{
				std::cout<<"no data!"<<std::endl;
			}
			std::string code = buffer.substr(buffer.find("code\":")+strlen("code\":"), buffer.find(",",buffer.find("code\":")+strlen("code\":"))-(buffer.find("code\":")+strlen("code\":")));
			//std::cout<<"tb_code--->"<<code<<std::endl;
			if( code == "0")
			{
				std::cout<<"success tb result!!!"<<std::endl;
			}
			else
			{
				std::cout<<"failed tb result"<<std::endl;
			}
			country = buffer.substr(buffer.find("country\":\"")+strlen("country\":\""), buffer.find("\"",buffer.find("country\":\"")+strlen("country\":\""))-(buffer.find("country\":\"")+strlen("country\":\"")));
			province = buffer.substr(buffer.find("region\":\"")+strlen("region\":\""), buffer.find("\"",buffer.find("region\":\"")+strlen("region\":\""))-(buffer.find("region\":\"")+strlen("region\":\"")));
			city = buffer.substr(buffer.find("city\":\"")+strlen("city\":\""), buffer.find("\"",buffer.find("city\":\"")+strlen("city\":\""))-(buffer.find("city\":\"")+strlen("city\":\"")));
			isp = buffer.substr(buffer.find("isp\":\"")+strlen("isp\":\""), buffer.find("\"",buffer.find("isp\":\"")+strlen("isp\":\""))-(buffer.find("isp\":\"")+strlen("isp\":\"")));
			//std::string tmp_ip = buffer.substr(buffer.find("ip\":\"")+strlen("ip\":\""), buffer.find("\"",buffer.find("ip\":\"")+strlen("ip\":\""))-(buffer.find("ip\":\"")+strlen("ip\":\"")));


			//
			std::string s1 = change_2_utf8(country);
			std::string s2 = change_2_utf8(province);
			std::string s3 = change_2_utf8(city);
			std::string s4 = change_2_utf8(isp);
			
			std::string target = current_ip + "   " + s1 + s2 + s3 + "   " + s4 + "\r\n";
			std::cout<<"target="<<target<<std::endl;
    			write(fd, (char*)target.data(), strlen(target.data()));
			
		}

		virtual void insert_db()
		{
			std::cout << "insert_db" << std::endl;
			std::cout<<"country="<<country<<std::endl;
			std::cout<<"province="<<province<<std::endl;
			std::cout<<"city="<<city<<std::endl;
			std::cout<<"isp="<<isp<<std::endl;
			std::cout<<"ip="<<ip<<std::endl;
		}

	public:
		std::string ip_begin; 
		std::string ip_end;
		char url[1024];
		char User_Agent[500];
		std::string buffer;
		char unRet;
		long code;
		std::string current_ip;

		std::string country;
		std::string province;
		std::string city;
		std::string isp;
		std::string ip;
		int fd;

};

//业务函数
void DoBussiness(spider_ip* spider)
{
	spider->make_spider();
	delete spider;
}

void test()
{
	DoBussiness(new tb_spider_ip("201.0.0.0","202.0.100.255"));
	//sleep(1);
}

int main()
{
	test();
	return 0;
}
