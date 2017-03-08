#ifndef ICR_ICSCHECK_HPP
#define ICR_ICSCHECK_HPP

#include <thread>
#include <mutex>
#include <string>
#include <vector>
#if 1

#define YES 1
#define NO  0
#define UNKNOW_FLAG  3
struct check_ip {
  std::string     chip;
	char       alive;
	char       oldalive;
};

class icscheck {
	public:
		static void master_worker(void *argptr);
		static void curl_worker(void *argptr, int check_ip_index);
		icscheck();
        std::vector<std::string> ip_list;
		virtual ~icscheck();

	private:
		void start();
		void alarm_threshold();
		void ip_list_set();
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
