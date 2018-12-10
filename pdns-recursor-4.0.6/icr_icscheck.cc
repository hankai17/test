#include <iostream>
#include <chrono>
#include <curl/curl.h>
#include <string.h>
#include "icr_icscheck.hh"
#include "recpacketcache.hh"
#include "logger.hh"

extern dns_conf* pconf;

void icscheck::init_ics()
{
	icr_icsploy_t * ics_ploy = new icr_icsploy_t[(*pconf).str_servers.size()];
	for(int i=0;i<(int)(*pconf).str_servers.size();i++)//多少组规则
	{
		ics_ploy_list.push_back(ics_ploy+i);//第一组规则地址已经 push进去
		ics_ploy[i].name = '0'+i;
		ics_ploy[i].ics_list = (*pconf).str_servers[i];
        ics_ploy[i].alive_ipv4_ics_count = ics_ploy[i].ics_list->ipv4_server.size();
        ics_ploy[i].alive_ipv6_ics_count = ics_ploy[i].ics_list->ipv6_server.size();
		
        icr_ip_t *ip4 = new icr_ip_t [(*(*pconf).str_servers[i]).ipv4_server.size()];                      
        icr_ip_t *ip6 = new icr_ip_t [(*(*pconf).str_servers[i]).ipv6_server.size()];
        (*ics_ploy_list[i]).ics_ipv4_array = new icr_ip_t* [(*(*pconf).str_servers[i]).ipv4_server.size()];
        (*ics_ploy_list[i]).ics_ipv6_array = new icr_ip_t* [(*(*pconf).str_servers[i]).ipv6_server.size()];
        for(int j=0;j<(int)(*(*pconf).str_servers[i]).ipv4_server.size();j++)//规则里有多少ip
        {                                                                                     
            ip4[j].ip = (*(*pconf).str_servers[i]).ipv4_server[j];
            ip4[j].unFlag = 1;
            ((*ics_ploy_list[i]).ics_ipv4_array)[j] = ip4 + j;
            ics_ip_str.push_back((*(*pconf).str_servers[i]).ipv4_server[j]);
        }

        for(int j=0;j<(int)(*(*pconf).str_servers[i]).ipv6_server.size();j++)//规则里有多少ip
        {
            ip6[j].ip = (*(*pconf).str_servers[i]).ipv6_server[j];
            ip6[j].unFlag = 1;
            ((*ics_ploy_list[i]).ics_ipv6_array)[j] = ip6 + j;
            ics_ip_str.push_back((*(*pconf).str_servers[i]).ipv6_server[j]);
        }
	}
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
		i++;
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

void icscheck::ipv4_moveto_back(int pos, icr_icsploy_t *icsploy) {
    icr_ip_t *tmp_ip = icsploy->ics_ipv4_array[pos];
    int i = pos;
    int ics_size = icsploy->ics_list->ipv4_server.size();
    for ( i = pos; i < ics_size - 1; i++) {
        icsploy->ics_ipv4_array[i] = icsploy->ics_ipv4_array[i+1];
    }   
    icsploy->ics_ipv4_array[i] = tmp_ip;
    if(icsploy->alive_ipv4_ics_count == 0) {
        return;
    }   
    icsploy->alive_ipv4_ics_count--;
}

void icscheck::ipv6_moveto_back(int pos, icr_icsploy_t *icsploy) {
    icr_ip_t *tmp_ip = icsploy->ics_ipv6_array[pos];
    int i = pos;
    int ics_size = icsploy->ics_list->ipv6_server.size();
    for ( i = pos; i < ics_size - 1; i++) {
        icsploy->ics_ipv6_array[i] = icsploy->ics_ipv6_array[i+1];
    }   
    icsploy->ics_ipv6_array[i] = tmp_ip;
    if(icsploy->alive_ipv6_ics_count == 0) {
        return;
    }   
    icsploy->alive_ipv6_ics_count--;
}

void icscheck::ipv4_moveto_head(int pos, icr_icsploy_t *icsploy) {
    icr_ip_t *tmp_ip = icsploy->ics_ipv4_array[pos];
    int i = pos;
    for ( i = pos; i > 0; i--) {
        icsploy->ics_ipv4_array[i] = icsploy->ics_ipv4_array[i-1];
    }   
    icsploy->ics_ipv4_array[i] = tmp_ip;
    icsploy->alive_ipv4_ics_count++;
    if(icsploy->alive_ipv4_ics_count > icsploy->ics_list->ipv4_server.size()) {
    //std::cout << icsploy->ploy_name<< " alive_ics_count greate than " << icsploy->ics_list.size() << std::endl;
    }   
}

void icscheck::ipv6_moveto_head(int pos, icr_icsploy_t *icsploy) {
    icr_ip_t *tmp_ip = icsploy->ics_ipv6_array[pos];
    int i = pos;
    for ( i = pos; i > 0; i--) {
        icsploy->ics_ipv6_array[i] = icsploy->ics_ipv6_array[i-1];
    }   
    icsploy->ics_ipv6_array[i] = tmp_ip;
    icsploy->alive_ipv6_ics_count++;
    if(icsploy->alive_ipv6_ics_count > icsploy->ics_list->ipv6_server.size()) {
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
				//TODO: reset icsploy
				for(auto icsploy : _self->ics_ploy_list) { 
                    int ipv4_ics_size = (int)icsploy->ics_list->ipv4_server.size();
                    int ipv6_ics_size = (int)icsploy->ics_list->ipv6_server.size();
                    for( auto j = 0; j < ipv4_ics_size; j++)
                    {
                        if(_check_ip_list[i].chip ==  icsploy->ics_ipv4_array[j]->ip) {
                            if(_check_ip_list[i].alive == NO) {
                                icsploy->ics_ipv4_array[j]->unFlag = 0;                
                                    _self->ipv4_moveto_back(j, icsploy);
                                
                            } else if (_check_ip_list[i].alive == YES) {
                                icsploy->ics_ipv4_array[j]->unFlag = 1;
                                        _self->ipv4_moveto_head(j, icsploy);           
                            }
                            break; // one ip is only apeared once in one ics ploy
                        }
                    }

                    for( auto j = 0; j < ipv6_ics_size; j++)
                    {
                        if(_check_ip_list[i].chip ==  icsploy->ics_ipv6_array[j]->ip) {
                            if(_check_ip_list[i].alive == NO) {
                                icsploy->ics_ipv6_array[j]->unFlag = 0;                
                                    _self->ipv6_moveto_back(j, icsploy);
                                
                            } else if (_check_ip_list[i].alive == YES) {
                                icsploy->ics_ipv6_array[j]->unFlag = 1;
                                        _self->ipv6_moveto_head(j, icsploy);           
                            }
                            break; // one ip is only apeared once in one ics ploy
                        }
                    }
				}
				{
                   //std::cout<<"ip "<<_check_ip_list[i].chip->data<<" server status change!"<<std::endl;
				    L<<Logger::Notice<< "Warning ics[" <<_check_ip_list[i].chip->data << "] server status change!"<<std::endl;
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
							"/etc/icdns/sdmail.pl \"\r\nICS[%s] is now back to normal!\"",
							checkptr->chip->data);
					system(command);
				}
				unFlag = 0;
			} else {
                L<<Logger::Notice<< "Warning ics[" << checkptr->chip->data << "] is can not connected!" <<std::endl;
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
				L<<Logger::Notice<< "Warning ics[" << checkptr->chip->data << "] is can not connected!" <<std::endl;
				}
				unFlag++;
				if( unFlag == _self->ics_alarm_threshold) {
					char command[128] = {0};
					snprintf(command, 128, "/etc/icdns/sdmail.pl \"\r\n Can not connect to ICS[%s]!\"", checkptr->chip->data);
					system(command);
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(3));
	}
}

char icscheck::curl_ics(icr_str_t *chip)
{
	char unRet = NO;

	char url[1024] =  {0}; //"http://%s/img/bdlogo.gif";
	char Host[500] = {0};
	memcpy(Host, "Host: www.17dao.com", strlen("Host: www.17dao.com"));
	//sprintf(url, "http://%s/noc/test1k", chip->data);
	memcpy(url, "http://", 7);
	memcpy(url+7, chip->data, chip->len);
	memcpy(url+7+chip->len, "/noc/test1k", strlen("/noc/test1k"));
//#define BUILD_DEBUG
#ifdef BUILD_DEBUG
	//WLOGF(DEBUG, "%s \n", url);
#endif

	CURL * curl = curl_easy_init(); 

	struct curl_slist *slist = NULL;
	slist = curl_slist_append(slist, Host);

	if (curl != NULL)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_ADDRESS_SCOPE, 0xEL); 
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

