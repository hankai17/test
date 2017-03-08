#include <iostream>
#include <chrono>
#include <curl/curl.h>
#include "icr_icscheck.hpp"


std::vector<std::vector<std::string>*> servers;
void icscheck::init_ics()
{

		std::vector<std::string>* ser1 = new std::vector<std::string>;
		std::vector<std::string>* ser2 = new std::vector<std::string>;
		ser1->push_back("10.10.110.141");
		ser1->push_back("10.10.110.142");
		ser1->push_back("10.10.110.143");
		ser1->push_back("10.10.110.48");
		ser2->push_back("10.10.110.180");
		ser2->push_back("10.10.110.181");
		ser2->push_back("10.10.110.182");
		ser2->push_back("10.10.110.183");
		servers.push_back(ser1);
		servers.push_back(ser2);
	std::cout<<"原始初始化完毕"<<std::endl;
	/**************************************/
	icr_icsploy_t * ics_ploy = new icr_icsploy_t[servers.size()];
	std::cout<<"servers.size()"<<servers.size()<<std::endl;
	for(int i=0;i<servers.size();i++)//多少组规则
	{
		ics_ploy_list.push_back(ics_ploy+i);//第一组规则地址已经 push进去
		ics_ploy[i].name = '0'+i;
		ics_ploy[i].ics_list = *servers[i];
		ics_ploy[i].alive_ics_count = ics_ploy[i].ics_list.size();
		
		icr_ip_t *ip = new icr_ip_t [(*servers[i]).size()];
		(*ics_ploy_list[i]).ics_ip_array = new icr_ip_t* [(*servers[i]).size()];
		for(int j=0;j<(*servers[i]).size();j++)//规则里有多少ip
		{
			ip[j].ip = (*servers[i])[j];
			ip[j].unFlag = 1;
		  ((*ics_ploy_list[i]).ics_ip_array)[j] = ip+j;
		  ics_ip_str.push_back((*servers[i])[j]);
		}	
	}
	
	//打印
	/*
	for(int i=0;i<servers.size();i++)//多少组规则
	{
		std::cout<<"ics_ploy[i].name "<<ics_ploy[i].name<<std::endl; 
		std::cout<<"ics_ploy[i].alive_ics_count"<<ics_ploy[i].alive_ics_count<<std::endl;
		
		std::cout<<"(*servers[i]).size()"<<(*servers[i]).size()<<std::endl;
		for(int j=0;j<(*servers[i]).size();j++)//规则里有多少ip
		{
			std::cout<<"(*servers[i])[j]"<<(*servers[i])[j]<<std::endl;
			//std::cout<<"ip[j].ip"<<ip[j].ip<<std::endl;
			//std::cout<<"ip[j].unFlag"<<ip[j].unFlag<<std::endl;
			std::cout<<"((*ics_ploy_list[i]).ics_ip_array)[j]"<<((*ics_ploy_list[i]).ics_ip_array)[j]->ip<<std::endl;
		}	
	}
	
	*/
	
	/*
	ics_ip_str.push_back("10.10.110.141");
	ics_ip_str.push_back("10.10.110.142");
	ics_ip_str.push_back("10.10.110.180");
	ics_ip_str.push_back("10.10.110.181");
	
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
	
	ics_ploy1->alive_ics_count = ics_ploy1->ics_list.size();
	ics_ploy2->alive_ics_count = ics_ploy2->ics_list.size();
	
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
	 	std::cout<<"初始化后第一个规则的,可用服务器个数:"<<(*ics_ploy_list[0]).alive_ics_count<<std::endl;
		std::cout<<"初始化后第二个规则的,可用服务器个数:"<<(*ics_ploy_list[1]).alive_ics_count<<std::endl;
*/
}

icscheck::icscheck()
{
	init_ics();
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
	ics_ip_num = ics_ip_str.size();
	check_ip_list = new check_ip[ics_ip_str.size()];
	int i = 0;
	for( auto itr = ics_ip_str.begin(); itr != ics_ip_str.end(); itr++) {
		check_ip_list[i].chip     = *itr;
		check_ip_list[i].alive    = YES;
		check_ip_list[i].oldalive = YES;
		std::cout<<check_ip_list[i].chip << " now insert into check ip list\n";
		i++;
	}
}

void icscheck::start()
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
	
	std::cout<<icsploy->name<<"组,正常服务器个数:"<<icsploy->alive_ics_count<<std::endl;
		
		
		
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
				std::cout<<"unFlag:"<<unFlag<<std::endl;
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
	std::cout<<"curl ip: "<<chip<<std::endl;
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
    std::cout<<"url="<<url<<std::endl;
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
