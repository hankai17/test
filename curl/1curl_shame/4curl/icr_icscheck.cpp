#include <iostream>
#include <chrono>
#include <curl/curl.h>
#include <string.h>
#include "icr_icscheck.hpp"

icscheck::icscheck() 
{
	//ip_list.push_back("127.0.0.1");
	ip_list.push_back("10.10.110.181");
	ip_list.push_back("10.10.110.59");
	ics_alarm_threshold = 0;
	alarm_threshold();
	ip_list_set();
	start();
}

icscheck::~icscheck()
{
	if(check_ip_list) {
		delete []check_ip_list;
		check_ip_list = nullptr;
	}
}

void icscheck::alarm_threshold()
{
	if(ics_alarm_threshold == 0) {
		ics_alarm_threshold = 10;
	} else if(ics_alarm_threshold < 0) {
		ics_alarm_threshold = 10000;
	}
}

void icscheck::ip_list_set()
{
	ics_ip_num = ip_list.size();
	check_ip_list = new check_ip[ics_ip_num];
    for(int i = 0; i < ics_ip_num; i++)
    {
		check_ip_list[i].chip     = ip_list[i];
		check_ip_list[i].alive    = YES;
		check_ip_list[i].oldalive = YES;
    }
}

void icscheck::start()
{
	std::thread master_thread(master_worker, this);
	m_mthread = std::move(master_thread);
	for(int i = 0; i < ics_ip_num; i++) {
		m_threads.push_back( (std::thread(curl_worker,this,i)) );
	}
}

void icscheck::master_worker(void *argptr)
{
	icscheck *_self = (icscheck *)argptr;
	auto _check_ip_list = _self->check_ip_list;
	while(1)
	{
        std::cout<<"in master_worker while"<<std::endl;
		for(auto i = 0; i < _self->ics_ip_num; i++) {
			if(_check_ip_list[i].alive != _check_ip_list[i].oldalive) {
							if(_check_ip_list[i].alive == NO) {
                            std::cout<<"server becoming err"<<std::endl;
							} 
                            else if (_check_ip_list[i].alive == YES) {
                            std::cout<<"server becoming well"<<std::endl;
							}
			}
			{
					std::unique_lock<std::mutex> lck(_self->m_datamutex);
					_check_ip_list[i].oldalive = _check_ip_list[i].alive;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void icscheck::curl_worker(void *argptr, int check_ip_index)
{
	icscheck *_self = (icscheck *)argptr;
	struct check_ip *checkptr = &_self->check_ip_list[check_ip_index];
	char curl_ret = checkptr->alive;
	int      unFlag = 0;
	while(1) {
        std::cout<<"......in curl_worker while"<<std::endl;
		curl_ret = _self->curl_ics(checkptr->chip); //假设能1s返回
		if(curl_ret != checkptr->alive) 
		{
			if( curl_ret == YES) {  
                std::cout<<"......server:"<<checkptr->chip<<" is can connect,send email..."<<std::endl;
			} else {
                std::cout<<"......server:"<<checkptr->chip<<" is can not connect,send email..."<<std::endl;
			}
			{
				std::unique_lock<std::mutex> lck(_self->m_datamutex);
				checkptr->alive = curl_ret;
			}
		}
		else
		{
			if (curl_ret != YES)
			{
                    std::cout<<"......server:"<<checkptr->chip<<" is can not connect,send email"<<std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

char icscheck::curl_ics(std::string &chip)
{
    std::cout<<".........curl探测"<<std::endl;
	char unRet = NO;

	char url[1024] =  {0}; //"http://%s/img/bdlogo.gif";
	char Host[500] = {0};
	memcpy(Host, "Host: www.17dao.com", strlen("Host: www.17dao.com"));
	//sprintf(url, "http://%s/noc/test1k", chip->data);
	memcpy(url, "http://", 7);
	memcpy(url+7, chip.data(), chip.length());
	memcpy(url+7+chip.length(), "/noc/test1k", strlen("/noc/test1k"));

    std::cout<<".........url:"<<url<<std::endl;
	CURL * curl = curl_easy_init(); //初始化

	struct curl_slist *slist = NULL;
	slist = curl_slist_append(slist, Host);

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
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL); // write_data param

		unRet = curl_easy_perform(curl);
		if (CURLE_OK == unRet)
		{
			unRet = YES;
			long code;
			curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
            std::cout<<".........response code:"<<code<<std::endl;
			if (code >= 400)
			{
				unRet = NO;
			}
		}
		else
		{
			unRet = NO;
		}
		curl_easy_cleanup(curl);
	}

	curl_slist_free_all(slist);

	return unRet;
}
size_t icscheck::write_data(__attribute__((unused)) void *buffer, size_t size, size_t nmemb, __attribute__((unused)) void *user_p)
{
	return size * nmemb;
}
