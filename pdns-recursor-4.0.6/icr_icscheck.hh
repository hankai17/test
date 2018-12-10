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
    uint32_t    unFlag; 
} icr_ip_t;

typedef struct _icr_icsploy_t {          
    std::string            name; 
    ip_obj*    ics_list;  
    icr_ip_t                   **ics_ipv4_array;	
    icr_ip_t                   **ics_ipv6_array;	
    uint32_t                   alive_ipv4_ics_count;
    uint32_t                   alive_ipv6_ics_count;
    uint32_t                   ics_count;
} icr_icsploy_t;


class icscheck {
    public:
        static void master_worker(void *argptr);
        static void curl_worker(void *argptr, int check_ip_index);
        icscheck();
        void init_ics();
        virtual ~icscheck();
        std::vector<icr_str_t*> ics_ip_str; 
        std::vector<icr_icsploy_t*> ics_ploy_list;

    private:
        void start();
        void alarm_threshold();
        void ip_list_set();
        void ipv4_moveto_back(int pos, icr_icsploy_t *icsploy);
        void ipv6_moveto_back(int pos, icr_icsploy_t *icsploy);
        void ipv4_moveto_head(int pos, icr_icsploy_t *icsploy); 
        void ipv6_moveto_head(int pos, icr_icsploy_t *icsploy); 
        char curl_ics(icr_str_t* chip);
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
