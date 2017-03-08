#include <iostream>
#include <chrono>
#include <curl/curl.h>
#include "icr_icscheck.hpp"



void icscheck::init_ics()
{
	//假设有2个组
	// std::vector< std::vector<std::string>* > ip_set;
/*
	std::vector<std::string> *ip_list1 = new std::vector<std::string> ;
	std::vector<std::string> *ip_list2 = new std::vector<std::string> ;
	ip_set.push_back(ip_list1);
	ip_set.push_back(ip_list2);
	(*ip_list1).push_back("10.10.110.141");
	(*ip_list1).push_back("10.10.110.142");
	
	(*ip_list2).push_back("10.10.110.180");
	(*ip_list2).push_back("10.10.110.181");
	/*
	for(int j=0;j<(int)ip_set[j]->size();j++)
	{
		for(int k=0;k<(int)(*ip_set[j]).size();k++)
		{
			ics_ip_str.push_back((*ip_set[j])[k]);
		}
	}
	*/
	ics_ip_str.push_back("10.10.110.141");
	ics_ip_str.push_back("10.10.110.142");
	ics_ip_str.push_back("10.10.110.180");
	ics_ip_str.push_back("10.10.110.181");
	//上面是我的结构体 初始化非常简单
	/////////////////////////////////////////////////////////
	//这是小亮的结构体 初始化非常多
	// std::vector<icr_icsploy_t*> ics_ploy_list;
	icr_icsploy_t * ics_ploy1 = new icr_icsploy_t;
	icr_icsploy_t * ics_ploy2 = new icr_icsploy_t;
	ics_ploy_list.push_back(ics_ploy1);
	ics_ploy_list.push_back(ics_ploy2);
	
	ics_ploy1->name = "group1";
	ics_ploy2->name = "group2";
	
	                   //ics_list初始化完毕
	(ics_ploy1->ics_list).push_back("10.10.110.141");
	(ics_ploy1->ics_list).push_back("10.10.110.142");
	
	
	(ics_ploy2->ics_list).push_back("10.10.110.180");
	(ics_ploy2->ics_list).push_back("10.10.110.181");
	
	
	//icr_ip_t                   **ics_ip_array;	
	icr_ip_t *ip1 = new icr_ip_t();  //icr_ip_t *ip1 = new icr_ip_t; 先new各ip的结构体
	ip1->ip = "10.10.110.141";

	icr_ip_t *ip2 = new icr_ip_t(); 
	ip2->ip = "10.10.110.142";
	
	icr_ip_t *ip3 = new icr_ip_t(); 
	ip3->ip = "10.10.110.180";
	
	icr_ip_t *ip4 = new icr_ip_t(); 
	ip4->ip = "10.10.110.181";
																	//交由各ics_ip_array管理
	(*ics_ploy_list[0]).ics_ip_array = new icr_ip_t* [2];
	((*ics_ploy_list[0]).ics_ip_array)[0] = ip1;
	((*ics_ploy_list[0]).ics_ip_array)[1] = ip2;
	
	(*ics_ploy_list[1]).ics_ip_array = new icr_ip_t* [2];
	((*ics_ploy_list[1]).ics_ip_array)[0] = ip3;
	((*ics_ploy_list[1]).ics_ip_array)[1] = ip4;
	 
	 
	 //初始化后打印
	 std::cout<<"初始化后 "<<((*ics_ploy_list[0]).ics_ip_array)[0]->ip<<std::endl;
	 std::cout<<"初始化后 "<<((*ics_ploy_list[0]).ics_ip_array)[1]->ip<<std::endl;
	 std::cout<<"初始化后 "<<((*ics_ploy_list[1]).ics_ip_array)[0]->ip<<std::endl;
	 std::cout<<"初始化后 "<<((*ics_ploy_list[1]).ics_ip_array)[1]->ip<<std::endl;
	 	
	
}

icscheck::icscheck() /*over*/
{
	init_ics();
	ics_alarm_threshold = 0;
	alarm_threshold();
	ip_list_set();
	start();
}

icscheck::~icscheck()/*over*/
{
	if(check_ip_list) {
		delete []check_ip_list;
		check_ip_list = nullptr;
	}
}

void icscheck::alarm_threshold() /*over*/
{
	if(ics_alarm_threshold == 0) {
		ics_alarm_threshold = 10;
	} else if(ics_alarm_threshold < 0) {
		ics_alarm_threshold = 10000;
	}
}

void icscheck::ip_list_set()/*over*/
{
	ics_ip_num = ics_ip_str.size();
	check_ip_list = new check_ip[ics_ip_str.size()];
	int i = 0;
	for( auto itr = ics_ip_str.begin(); itr != ics_ip_str.end(); itr++) {
		check_ip_list[i].chip     = *itr;
		check_ip_list[i].alive    = YES;
		check_ip_list[i].oldalive = YES;
		//WLOG(DEBUG) << check_ip_list[i].chip->data << " insert into check ip list\n";
		std::cout<<check_ip_list[i].chip << " now insert into check ip list\n";
		i++;
	}
}

void icscheck::start()/*over*/
{
	std::thread master_thread(master_worker, this);
	m_mthread = std::move(master_thread);
	for(int i = 0; i < ics_ip_num; i++) {
		m_threads.push_back(std::thread(curl_worker, this, i));
	}
}



void icscheck::moveto_back(int pos, icr_icsploy_t *icsploy) {
	icr_ip_t *tmp_ip = icsploy->ics_ip_array[pos];
	int i = pos;
	int ics_size = icsploy->ics_list.size();
	for ( int j = 0; j < ics_size; j++) {
	std::cout<<"移到队尾前 "<<icsploy->ics_ip_array[j]->ip<<std::endl;
	}
	for ( i = pos; i < ics_size - 1; i++) {
		icsploy->ics_ip_array[i] = icsploy->ics_ip_array[i+1];
	}
	icsploy->ics_ip_array[i] = tmp_ip;
	
	for ( int j = 0; j < ics_size; j++) {
	std::cout<<"移到队尾后 "<<icsploy->ics_ip_array[j]->ip<<std::endl;
	}
	if(icsploy->alive_ics_count == 0) {
		return;
	}
	icsploy->alive_ics_count--;
	
	
}

void icscheck::moveto_head(int pos, icr_icsploy_t *icsploy) {
	icr_ip_t *tmp_ip = icsploy->ics_ip_array[pos];
	int i = pos;
	for ( i = pos; i > 0; i--) {
		icsploy->ics_ip_array[i] = icsploy->ics_ip_array[i-1];
	}
	icsploy->ics_ip_array[i] = tmp_ip;
	icsploy->alive_ics_count++;
	int ics_size = icsploy->ics_list.size();
	for ( int j = 0; j < ics_size; j++) {
	std::cout<<"移到队头后 "<<icsploy->ics_ip_array[j]->ip<<std::endl;
	}
	if(icsploy->alive_ics_count > icsploy->ics_list.size()) {
		//WLOG(WARNING) << icsploy->ploy_name.data << " alive_ics_count greate than " << icsploy->ics_list.size() << std::endl;
	//std::cout << icsploy->ploy_name<< " alive_ics_count greate than " << icsploy->ics_list.size() << std::endl;

	}
	
}

void icscheck::master_worker(void *argptr)
{
	icscheck *_self = (icscheck *)argptr;
	auto _check_ip_list = _self->check_ip_list;
	while(1)
	{
		for(auto i = 0; i < _self->ics_ip_num; i++) {
			if(_check_ip_list[i].alive != _check_ip_list[i].oldalive) {
				std::cout<<_self->check_ip_list[i].chip << " insert into check ip list\n";
				//TODO: reset icsploy
				for(auto icsploy : _self->ics_ploy_list) { 
					int ics_size = (int)icsploy->ics_list.size();
					for( auto j = 0; j < ics_size; j++)
					{
						if(_check_ip_list[i].chip ==  icsploy->ics_ip_array[j]->ip) {
							std::cout<<"ip "<<_check_ip_list[i].chip<<"前后状态不等";
							if(_check_ip_list[i].alive == NO) {
								std::cout<<"将移动到队尾"<<std::endl;			
								icsploy->ics_ip_array[j]->unFlag = 0;						
									_self->moveto_back(j, icsploy);
								
							} else if (_check_ip_list[i].alive == YES) {
								icsploy->ics_ip_array[j]->unFlag = 1;
								std::cout<<"将移动到队头"<<std::endl;
										_self->moveto_head(j, icsploy);							
							}
							break; // one ip is only apeared once in one ics ploy
						}
					}
				}
				{
					std::unique_lock<std::mutex> lck(_self->m_datamutex);
					_check_ip_list[i].oldalive = _check_ip_list[i].alive;
				}
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
	int  unFlag = 0;
	while(1) {
		curl_ret = _self->curl_ics(checkptr->chip);
		if(curl_ret != checkptr->alive)
		{
			if( curl_ret == YES) {
				if(unFlag >= _self->ics_alarm_threshold) {
					char command[128] = {0};
					snprintf(command, 128,
							"/etc/icrskice/sdmail.pl \"\r\nICS[%s] is now back to normal!\"",
							checkptr->chip);
					system(command);
				}
				unFlag = 0;
			} else {
				//WLOG(WARNING) << "ics[" << checkptr->chip->data << "] is can not connected!\n";
				std::cout << "ics[" << checkptr->chip << "] is can not connected!\n";
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
					//WLOGF(WARNING, "ics:[%s] can not connected!\n", checkptr->chip->data);
				}
				unFlag++;
				if( unFlag == _self->ics_alarm_threshold) {
					char command[128] = {0};
					//snprintf(command, 128, "/etc/icrskice/sdmail.pl \"\r\n Can not connect to ICS[%s]!\"", checkptr->chip->data);
					//system(command);
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

char icscheck::curl_ics(std::string chip)
{
	char unRet = NO;

	char url[1024] =  {0}; //"http://%s/img/bdlogo.gif";
	char Host[500] = {0};
	memcpy(Host, "Host: www.17dao.com", strlen("Host: www.17dao.com"));
/*
	if (chip == nullptr)
	{
		return NO;
	}
*/
	//sprintf(url, "http://%s/noc/test1k", chip->data);
	memcpy(url, "http://", 7);
	memcpy(url+7, chip.data(), chip.length());
	memcpy(url+7+chip.length(), "/noc/test1k", strlen("/noc/test1k"));
//#define BUILD_DEBUG
#ifdef BUILD_DEBUG
	//WLOGF(DEBUG, "%s \n", url);
#endif

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
#ifdef BUILD_DEBUG
			WLOGF(DEBUG, "response code: %ld\n", code);
#endif
			if (code >= 400)
			{
				unRet = NO;
			}
			std::cout<<chip<<" response code: "<<code<<std::endl;
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
