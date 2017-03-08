#ifndef ICR_ICSCHECK_HPP
#define ICR_ICSCHECK_HPP

#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include<string.h>
#include <vector>
#if 1
#define   INVALIDIP      0xFFFFFFFF
#define   INIT_MASK      0xFFFFFFFF
#define  YES                    1
#define  NO                     0
#define  UNKOWN_FLAG            3

struct check_ip {
	std::string chip;
	char  alive;
	char  oldalive;
};

typedef struct _icr_ip_t {
	//in_addr_t          ip;
	std::string          ip;   
	char               ip_addr[5];
	char               *ip_addr2;
	in_addr_t          ip_mask;

	uint32_t    unFlag; // no use
	std::string   *stServerIpx;
	_icr_ip_t()
	{
		ip       = INVALIDIP;
		ip_mask  = INIT_MASK;
		ip_addr2 = nullptr;
		bzero(ip_addr, 5);
		unFlag = 1;
		stServerIpx = nullptr;
	}
} icr_ip_t;

typedef struct _icr_icsploy_t {           //һһ��ʼ�� ......
	std::string            name; 
	std::vector<std::string>    ics_list;  
	icr_ip_t                   **ics_ip_array;	
																					 // new *icr_ip_t, icr_ip_tָ��ָ�� �Ѿ�����õ��ڴ�ռ�,�ǳ��п�����һ��hash��ַ �����������ַ
																					 // ics_ip_array ���ڹ���һ��ָ������
	int                        *ics_alive_index_array;//Ҫ
	uint32_t                   alive_ics_count;//Ҫ
	uint32_t                   ics_count;

	std::vector<std::string*>     ics_ip_list; 

	uint32_t unOKIcsNum;
	/*
	_icr_icsploy_t(std::string _ploy_name) {
		ploy_name = _ploy_name;
	}
	
	~_icr_icsploy_t() {
		if(ploy_name.data != nullptr ) {
			delete []ploy_name.data;
			ploy_name.data = nullptr;
		}
	}
	*/
} icr_icsploy_t;

class icscheck {
	public:
		static void master_worker(void *argptr);
		static void curl_worker(void *argptr, int check_ip_index);
		icscheck();
		void init_ics();
		virtual ~icscheck();
	  std::vector<std::string> ics_ip_str;//���е�ip���Ž�ȥ
	  std::vector<icr_icsploy_t*> ics_ploy_list;//С���Ľṹ:2�������� �ܶ���� 
	  std::vector< std::vector<std::string>* > ip_set; //�ҵĽṹ:2��������
	  check_ip                 *check_ip_list;	
	private:
		void start();
		void alarm_threshold();
		void ip_list_set();
		//void remove_icsindex(int pos, icr_icsploy_t *icsploy);
		//void insert_icsindex(int pos, icr_icsploy_t *icsploy);
		void moveto_back(int pos, icr_icsploy_t *icsploy);
		void moveto_head(int pos, icr_icsploy_t *icsploy);
		char curl_ics(std::string chip);
		static size_t write_data(__attribute__((unused)) void *buffer, size_t size, size_t nmemb, __attribute__((unused)) void *user_p);

	private:
		int                  ics_alarm_threshold;
		std::vector<std::thread> m_threads;
		std::thread              m_mthread;
		std::mutex               m_datamutex;
		
		
		int                  ics_ip_num;
};
#endif
#endif
