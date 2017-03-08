#include <iostream>
#include <chrono>
#include <curl/curl.h>
#include "icr_log/icr_log.hpp"
#include "icr_status/icr_icscheck.hpp"

icscheck::icscheck(icr_conf *_confptr) 
{
	ConfPtr = _confptr;
	MainPtr = ConfPtr->main_ctx;
	ics_alarm_threshold = MainPtr->ics_alarm_threshold;
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


void icscheck::ip_list_set()
{
	ics_ip_num = MainPtr->srv_conf.ics_ip_str_hash.size();
	check_ip_list = new check_ip[MainPtr->srv_conf.ics_ip_str_hash.size()];
	int i = 0;
	for( auto itr = MainPtr->srv_conf.ics_ip_str_hash.begin(); itr != MainPtr->srv_conf.ics_ip_str_hash.end(); itr++) {
		check_ip_list[i].chip     = (icr_str_t *)itr->second;
		check_ip_list[i].alive    = YES;
		check_ip_list[i].oldalive = YES;
		WLOG(DEBUG) << check_ip_list[i].chip->data << " insert into check ip list\n";
		i++;
	}
}


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

void icscheck::master_worker(void *argptr)
{
	icscheck *_self = (icscheck *)argptr;
	auto SrvPtr = &_self->MainPtr->srv_conf;
	auto _check_ip_list = _self->check_ip_list;
	while(1)
	{
		for(auto i = 0; i < _self->ics_ip_num; i++) {
			if(_check_ip_list[i].alive != _check_ip_list[i].oldalive) {
				//WLOG(DEBUG) << _check_ip_list[i].chip->data << " alive : " << (int)_check_ip_list[i].alive << ", oldalive : " << (int)_check_ip_list[i].oldalive << "\n";
				//TODO: reset icsploy
				for(auto icsploy : SrvPtr->ics_ploy_list) {
					int ics_size = (int)icsploy->ics_list.size();
					for( auto j = 0; j < ics_size; j++)
					{
						//WLOG(DEBUG) << icsploy->ploy_name.data << " s icsip[" << j << "] : " << icsploy->ics_ip_array[j]->stServerIpx->data << ", check ip : " << _check_ip_list[i].chip->data << "\n";
						if(strcmp(icsploy->ics_ip_array[j]->stServerIpx->data, _check_ip_list[i].chip->data) == 0) {
							if(_check_ip_list[i].alive == NO) {
								//WLOG(DEBUG) << icsploy->ploy_name.data << " i icsip[" << j << "] : " << icsploy->ics_ip_array[j]->stServerIpx->data << ", check ip : " << _check_ip_list[i].chip->data << "\n";
								icsploy->ics_ip_array[j]->unFlag = 0;
								if ( SrvPtr->use_consistent_hash == YES) {
									_self->remove_icsindex(j, icsploy);
								} else {
									//WLOG(WARNING) << "NO move " << i << " : " << _check_ip_list[i].chip->data << " to back\n";
									_self->moveto_back(j, icsploy);
								}
							} else if (_check_ip_list[i].alive == YES) {
								icsploy->ics_ip_array[j]->unFlag = 1;
								if ( SrvPtr->use_consistent_hash == YES) {
									_self->insert_icsindex(j, icsploy);
								} else {
									//WLOG(WARNING) << "YES move " << i << " : " << _check_ip_list[i].chip->data << " to head\n";
									_self->moveto_head(j, icsploy);
								}
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




