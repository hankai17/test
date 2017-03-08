#ifndef ICR_ICSCHECK_HPP
#define ICR_ICSCHECK_HPP

#include <thread>
#include <mutex>
#include "icr_common/icr_conf_file.hpp"
#if 1

struct check_ip {
	icr_str_t *chip;
	icr_flag  alive;
	icr_flag  oldalive;
};

class icscheck {
	public:
		static void master_worker(void *argptr);
		static void curl_worker(void *argptr, int check_ip_index);
		icscheck(icr_conf *_confptr);
		virtual ~icscheck();

	private:
		void start();
		void alarm_threshold();
		void ip_list_set();
		void remove_icsindex(int pos, icr_icsploy_t *icsploy);
		void insert_icsindex(int pos, icr_icsploy_t *icsploy);
		void moveto_back(int pos, icr_icsploy_t *icsploy);
		void moveto_head(int pos, icr_icsploy_t *icsploy);
		icr_flag curl_ics(icr_str_t *chip);
		static size_t write_data(__attribute__((unused)) void *buffer, size_t size, size_t nmemb, __attribute__((unused)) void *user_p);

	private:
		icr_int                  ics_alarm_threshold;
		std::vector<std::thread> m_threads;
		std::thread              m_mthread;
		std::mutex               m_datamutex;
		icr_conf                 *ConfPtr;
		icr_main_t               *MainPtr;

		check_ip                 *check_ip_list;
		icr_int                  ics_ip_num;
};
#endif
#endif
