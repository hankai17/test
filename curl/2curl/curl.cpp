#include <iostream>
#include <chrono>
#include <curl/curl.h>
#include "icr_icscheck.hpp"
icscheck::icscheck() 
{
	ip_list.push_back("127.0.0.1");
  ip_list.push_back("10.10.110.161");
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
		m_threads.push_back(std::thread(curl_worker, this, i));
	}
}
/*
void icscheck::remove_icsindex(int pos, icr_icsploy_t *icsploy) {
	int ics_size = icsploy->ics_list.size();
	bool remove_ok = false;
	int i = 0;
	for ( i = 0; i < ics_size - 1; i++ ) {
		if ( pos == icsploy->ics_alive_index_array[i])
		{
			icsploy->ics_alive_index_array[i] = icsploy->ics_alive_index_array[i+1];
			remove_ok = true;
		}
		else if ( remove_ok )
		{
			icsploy->ics_alive_index_array[i] = icsploy->ics_alive_index_array[i+1];
		}
	}
	if ( remove_ok ) {
		icsploy->ics_alive_index_array[i] = pos;
	}
	if ( icsploy->alive_ics_count != 0) {
		icsploy->alive_ics_count--;
	}
}

void icscheck::insert_icsindex(int pos, icr_icsploy_t *icsploy) {
	int ics_size = icsploy->ics_list.size();
	int alive_count = static_cast<int>(icsploy->alive_ics_count);
	if ( alive_count >= ics_size) {
		return;
	}

	if ( alive_count == 0) {
		icsploy->ics_alive_index_array[0] = pos;
	} else if ( icsploy->ics_alive_index_array[alive_count - 1] < pos) {
		icsploy->ics_alive_index_array[alive_count] = pos;
	} else {
		int i = 0;
		bool insert_ok = false;
		for ( i = alive_count-1; i > 0; i-- ) {
			icsploy->ics_alive_index_array[i+1] = icsploy->ics_alive_index_array[i];
			if ( pos > icsploy->ics_alive_index_array[i-1] && pos < icsploy->ics_alive_index_array[i])
			{
				icsploy->ics_alive_index_array[i] = pos;
				insert_ok = true;
				break;
			}
		}
		if (!insert_ok) {
			icsploy->ics_alive_index_array[i+1] = icsploy->ics_alive_index_array[i];
			icsploy->ics_alive_index_array[i] = pos;
		}
	}
	icsploy->alive_ics_count++;
}

void icscheck::moveto_back(int pos, icr_icsploy_t *icsploy) {
	icr_ip_t *tmp_ip = icsploy->ics_ip_array[pos];
	int i = pos;
	int ics_size = icsploy->ics_list.size();
	for ( i = pos; i < ics_size - 1; i++) {
		icsploy->ics_ip_array[i] = icsploy->ics_ip_array[i+1];
	}
	icsploy->ics_ip_array[i] = tmp_ip;
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
	if(icsploy->alive_ics_count > icsploy->ics_list.size()) {
		WLOG(WARNING) << icsploy->ploy_name.data << " alive_ics_count greate than " << icsploy->ics_list.size() << std::endl;
	}
}
*/
void icscheck::master_worker(void *argptr)
{
	icscheck *_self = (icscheck *)argptr;
	auto _check_ip_list = _self->check_ip_list;
	while(1)
	{
		for(auto i = 0; i < _self->ics_ip_num; i++) {
			if(_check_ip_list[i].alive != _check_ip_list[i].oldalive) {
				//WLOG(DEBUG) << _check_ip_list[i].chip->data << " alive : " << (int)_check_ip_list[i].alive << ", oldalive : " << (int)_check_ip_list[i].oldalive << "\n";
				//TODO: reset icsploy
					for( auto j = 0; j <_self->ics_ip_num; j++)
					{
							if(_check_ip_list[i].alive == NO) {
                            //move ...
                            NULL;
							} else if (_check_ip_list[i].alive == YES) {
                            //move ...
                            NULL;
							}
							break; // one ip is only apeared once in one ics ploy
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
	icr_flag curl_ret = checkptr->alive;
	int      unFlag = 0;
	while(1) {
		curl_ret = _self->curl_ics(checkptr->chip);
		if(curl_ret != checkptr->alive) //当前状态与alive状态不同
		{
			if( curl_ret == YES) {  //当前为yes  服务器活了
				if(unFlag >= _self->ics_alarm_threshold) {
                std::cout<<"服务器活了"<<std::endl;
                /*
					char command[128] = {0};
					snprintf(command, 128,
							"/etc/icrskice/sdmail.pl \"\r\nICS[%s] is now back to normal!\"",
							checkptr->chip->data);
					system(command);
                    */
				}
				unFlag = 0;
			} else {
				//WLOG(WARNING) << "ics[" << checkptr->chip->data << "] is can not connected!\n";
                std::cout<<"server:"<<checkptr->chip<<"is can not connect"<<std::endl;
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
                    std::cout<<"server:"<<checkptr->chip<<"is can not connect"<<std::endl;
				}
				unFlag++;
				if( unFlag == _self->ics_alarm_threshold) {
                    std::cout<<"server:"<<checkptr->chip<<"is can not connect"<<std::endl;
                /*
					char command[128] = {0};
					snprintf(command, 128, "/etc/icrskice/sdmail.pl \"\r\n Can not connect to ICS[%s]!\"", checkptr->chip->data);
					system(command);
                    */
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

char icscheck::curl_ics(std::string &chip)
{
	char unRet = NO;

	char url[1024] =  {0}; //"http://%s/img/bdlogo.gif";
	char Host[500] = {0};
	memcpy(Host, "Host: www.17dao.com", strlen("Host: www.17dao.com"));
/*
	if (chip == nullptr || chip->data == nullptr)
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
    std::cout<<"url:"<<url<<std::endl;
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
