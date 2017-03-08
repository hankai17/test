#include <iostream>
#include <chrono>
#include <curl/curl.h>
#include <string.h>
#include "icr_icscheck.hh"

#include "recpacketcache.hh"

extern std::vector<std::vector<std::string>*> servers;
int clear_flag = 0;

icscheck::icscheck() 
{ 
	//假设有2个组
	std::vector<std::string> *ip_list1 = new std::vector<std::string> ;
	std::vector<std::string> *ip_list2 = new std::vector<std::string> ;
	ip_set.push_back(ip_list1);
	ip_set.push_back(ip_list2);
	(*ip_list1).push_back("10.10.110.1");
	(*ip_list1).push_back("10.10.110.2");
	
	(*ip_list2).push_back("10.10.110.3");
	(*ip_list2).push_back("10.10.110.4");
		
	
	/*
    for(int i=0;i<(int)(servers.size());i++)
    {
        for(std::vector<std::string>::iterator it=(*servers[i]).begin(); it != (*servers[i]).end(); it++)
        {
           std::cout<< *it<<std::endl;
	       ip_list.push_back(*it);
        }
    }
    for(std::vector<std::string>::iterator it1=ip_list.begin(); it1 != ip_list.end(); it1++)
    {
        std::cout<<*it1<<std::endl;
    }
    */
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

//std::vector< std::vector<std::string>* > ip_set;
void icscheck::ip_list_set()
{
	for(int i=0;i<(int)ip_set.size();i++)
	{
		ics_ip_num+=(int)(*ip_set[i]).size();
	}
	check_ip_list = new check_ip[ics_ip_num];
	
	for(int j=0;j<(int)ip_set[j].size();j++)
	{
		for(int k=0;k<(int)(*ip_set[i]).size();k++)
		{
			check_ip_list[i].chip = (*ip_set[i])[k];
			check_ip_list[i].alive    = YES;
			check_ip_list[i].oldalive = YES;
		}
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
        //std::cout<<"in master_worker while"<<std::endl;
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
/*
void icscheck::curl_worker(void *argptr, int check_ip_index)
{
    //sleep(20);
    //std::cout<<"sleep 20s over,begin curl......"<<std::endl;
	icscheck *_self = (icscheck *)argptr;
	struct check_ip *checkptr = &_self->check_ip_list[check_ip_index];
	char curl_ret = checkptr->alive;
	//int      unFlag = 0;
	while(1) {
        //std::cout<<"......in curl_worker while"<<std::endl;
		curl_ret = _self->curl_ics(checkptr->chip); //假设能1s返回
		if(curl_ret != checkptr->alive) 
		{
			if( curl_ret == YES) {  
                std::cout<<"server:"<<checkptr->chip<<" is can connect,send email..."<<std::endl;
			} else {
                std::cout<<"server:"<<checkptr->chip<<" is can not connect,send email..."<<std::endl;
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
                    std::cout<<"server:"<<checkptr->chip<<" is can not connect,send email"<<std::endl;
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}
*/
void icscheck::curl_worker(void *argptr, int check_ip_index)
{
	icscheck *_self = (icscheck *)argptr;
	struct check_ip *checkptr = &_self->check_ip_list[check_ip_index];
	icr_flag curl_ret = checkptr->alive;
	icr_int  unFlag = 0;
	while(1) {
		curl_ret = _self->curl_ics(checkptr->chip);
		if(curl_ret != checkptr->alive)
		{
			if( curl_ret == YES) {
				if(unFlag >= _self->ics_alarm_threshold) {
					char command[128] = {0};
					snprintf(command, 128,
							"/etc/icrskice/sdmail.pl \"\r\nICS[%s] is now back to normal!\"",
							checkptr->chip->data);
					system(command);
				}
				unFlag = 0;
			} else {
				//WLOG(WARNING) << "ics[" << checkptr->chip->data << "] is can not connected!\n";
				unFlag++;
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
				if(unFlag % 3 == 0) {
					//WLOGF(WARNING, "ics:[%s] can not connected!\n", checkptr->chip->data);
				}
				unFlag++;
				if( unFlag == _self->ics_alarm_threshold) {
					char command[128] = {0};
					snprintf(command, 128, "/etc/icrskice/sdmail.pl \"\r\n Can not connect to ICS[%s]!\"", checkptr->chip->data);
					system(command);
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}
char icscheck::curl_ics(std::string &chip)
{
    //std::cout<<".........curl探测"<<std::endl;
	char unRet = NO;

	char url[1024] =  {0}; //"http://%s/img/bdlogo.gif";
	char Host[500] = {0};
	memcpy(Host, "Host: www.17dao.com", strlen("Host: www.17dao.com"));
	//sprintf(url, "http://%s/noc/test1k", chip->data);
	memcpy(url, "http://", 7);
	memcpy(url+7, chip.data(), chip.length());
	memcpy(url+7+chip.length(), "/noc/test1k", strlen("/noc/test1k"));

    //std::cout<<".........url:"<<url<<std::endl;
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
            std::cout<<chip<<" response code:"<<code<<std::endl;
			if (code >= 400)
			{
               //int count=t_packetCache->domyWipePacketCache();
               //std::cout<<"!!!!!!!!!!!!!!clear all cache over"<<std::endl; 
               //std::cout<<"!!!!!!!!!!!!!!count:"<<count<<std::endl; 
				unRet = NO;
			}
		}
		else
		{
			unRet = NO;
		}

        if( unRet == NO)
        {
            //clear_flag = 1;
            std::cout<<"curl... clear_flag: "<<clear_flag<<std::endl; 
        /*
            std::cout<<"unRet == NO"<<std::endl; 
            std::cout<<"clear_cache_flag: "<<clear_cache_flag<<std::endl; 
            std::cout<<"add:"<<t_packetCache<<std::endl;
            if(clear_cache_flag == 1)
            {
              t_packetCache->domyWipePacketCache();
              std::cout<<"!!!!!!!!!!!!!!clear all cache over"<<std::endl; 
              //clear_cache_flag=0;
            }
            */
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
