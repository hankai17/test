#ifndef ICR_ICSCHECK_HPP
#define ICR_ICSCHECK_HPP

#include <thread>
#include <mutex>
#include <string>
#include <vector>
#include "conf.hh"
#if 1

#define YES 1
#define NO  0
#define UNKNOW_FLAG  3
struct check_ip {
  icr_str_t*     chip;
	char       alive;
	char       oldalive;
};

typedef struct _icr_ip_t {
	icr_str_t*          ip;   
	uint32_t    unFlag; // no use
} icr_ip_t;

typedef struct _icr_icsploy_t {          
	std::string            name; 
	std::vector<icr_str_t*>    ics_list;  
	icr_ip_t                   **ics_ip_array;	
																					 // new *icr_ip_t, icr_ip_t指针指向 已经分配好的内存空间,非常有可能是一个hash地址 而不是数组地址
																					 // ics_ip_array 用于管理一级指针数组
	uint32_t                   alive_ics_count;
	uint32_t                   ics_count;
} icr_icsploy_t;


class icscheck {
	public:
		static void master_worker(void *argptr);
		static void curl_worker(void *argptr, int check_ip_index);
		icscheck();
		void init_ics();
		virtual ~icscheck();
		std::vector<icr_str_t*> ics_ip_str; //全部的ip
		std::vector<icr_icsploy_t*> ics_ploy_list;//小亮的结构:2个规则组 很多参数 
		

	private:
		void start();
		void alarm_threshold();
		void ip_list_set();
		void moveto_back(int pos, icr_icsploy_t *icsploy);
		void moveto_head(int pos, icr_icsploy_t *icsploy); 
		char curl_ics(std::string &chip);
		static size_t write_data(__attribute__((unused)) void *buffer, size_t size, size_t nmemb, __attribute__((unused)) void *user_p);

	private:
		int                      ics_alarm_threshold;
		std::vector<std::thread> m_threads;
		std::thread              m_mthread;
		std::mutex               m_datamutex;

		check_ip                 *check_ip_list;
	  int                      ics_ip_num;
};
#endif
#endif
