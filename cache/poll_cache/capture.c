#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/select.h>
#include <poll.h>
#include "prot.h"
#include "queue.h"
#include "cmn_log.h"
#include "capture_poll.h"
#define LOG_ERR1          LOG_LOCAL3
#define LOAD_LEN   2048
#define MAX_DNS_THREAD_NUM      100
#define MAX_HTTP_THREAD_NUM     100
FILE *fp_out = NULL;
char path_server_ip[60];
char dev_name[60] = { 0 };
char *insert_name = NULL;
rule *white_rule;
rule *black_rule;
rule *second_filter;
rule *src_ip_white;
rule *src_ip_black;
rule *http_white_list;
rule *http_black_list;
rule *server_ip;
rule *keywords_filter;
int now_count_white = 0;
int max_count_white = 0;
int now_count_black = 0;
int max_count_black = 0;
int now_count_second_filter = 0;
int max_count_second_filter = 0;
int now_count_ip_white = 0;
int max_count_ip_white = 0;
int now_count_ip_black = 0;
int max_count_ip_black = 0;
int max_count_http_white_list = 0;
int max_count_http_black_list = 0;
int now_count_http_black_list = 0;
int now_count_http_white_list = 0;
int now_count_server_ip = 0;
int max_count_server_ip = 0;
int now_count_keywords_filter = 0;
int max_count_keywords_filter = 0;
//��ǰ��dns���鿴�ٶ��Ƿ���ǰ
int dns_content[4] = { 0x01000cc0, 0x00000100, 0x0400ee01, 0x0 };
pcap_t *pcap_filter_ready(const char *dev, const char *str, char *errbuff) {
    bpf_u_int32 ipaddress, ipmask;
    struct bpf_program bp;
    pcap_t *cp = NULL;
    cp = pcap_open_live(dev, 65535, 0, 1, errbuff); //overtime  1ms
    if (cp == NULL) {
        perror("open failed!");
        printf("%s\n", errbuff);
    } else if (errbuff[0] != 0) {
        printf("%s\n", errbuff);
        return NULL;
    }
    if (pcap_lookupnet(dev, &ipaddress, &ipmask, errbuff) < 0) {
        ipmask = 0;
        ipaddress = 0;
        perror("pcap_lookupnet failed!\n");
        return NULL;
    }
    if (pcap_compile(cp, &bp, str, 0, ipmask) == -1) {
        printf("%s\n", pcap_geterr(cp));
        return NULL;
    }
    /*****set filter****/
    if (pcap_setfilter(cp, &bp) == -1) {
        printf("%s\n", pcap_geterr(cp));
        return NULL;
    }
    return cp;
}
static void sigroutine(int dunno)
{ /* ?źŴ??????̣?????dunno?????õ??źŵ?ֵ */ 
    pthread_mutex_lock(&g_shutdown_mutex);
    g_nShutDown = CAPTURE_SHUTDOWN_YES;
    pthread_mutex_unlock(&g_shutdown_mutex);
    CMN_LOG(MY_LOG_DEBUG, "capture stop exit!\n");
    return; 
} 
int main(int argc, char *argv[]) {
    //  int dns_choose = 0 ;
    int function_choose = 0;
    char CURR_PATH[1024] = { 0 }; //get current path
    char LOG_PATH[1024] = { 0 };
    //   if(argc >= 2)
    //     dns_choose  =1;//handle the dns request!
    CF conf_file;
    char str_dns[1024] = { 0 };
    char str_http[1024] = { 0 };
    char errbuf[BUFFER];
    pcap_t *cp[2] = { NULL, NULL };
    char *port = NULL;
    char *dev = NULL;
    //    bpf_u_int32 ipaddress,ipmask;
    //struct bpf_program bp;
    int white_fd;
    int black_fd;
    int filter_fd;
    pcap_handler callback[2] = { dns_handler, http_handler };
    int dns_thread_num = 0;
    int http_thread_num = 0;
    //char str[1024] = " tcp dst port 80 and host 101.71.74.235 and tcp[tcpflags]&(tcp-push)!=0 ";
    // char str[1024] = " tcp dst port 80 and tcp[tcpflags]&(tcp-push)!=0 ";
    memset(errbuf, 0, BUFFER);
    /*
     *init the argument :port , dev .....
     */
    //open syslog
    //open("redirect_log", LOG_PID, LOG_ERR1);
    //my_daemon();
    if (get_conf_value("cache.conf", &conf_file) == -1) {
        CMN_LOG(MY_LOG_DEBUG, "can't find the cache.conf error!\n");
        exit(1);
    }
    port = conf_file.port;
    dev = conf_file.dev;
    function_choose = atoi(conf_file.function);   //get the function of choose
    strcpy(path_server_ip, conf_file.path_server_ip);
    strcpy(dev_name, dev);
    dns_thread_num = atoi(conf_file.dns_thread_num);
    http_thread_num = atoi(conf_file.http_thread_num);
    dns_thread_num = dns_thread_num > MAX_DNS_THREAD_NUM ? MAX_DNS_THREAD_NUM : dns_thread_num;
    http_thread_num = http_thread_num > MAX_HTTP_THREAD_NUM ? MAX_HTTP_THREAD_NUM : http_thread_num;
    insert_name = conf_file.insert_name;
    /*
     *strcpy(white_path,conf_file.path_white);
     *strcpy(black_path,conf_file.path_black);
     *strcpy(path_second_filter, conf_file.path_second_filter);
     *���ļ����ݵ����ڴ浱�У���߶�ȡ�ٶ�
     */
#if 1
    if (read_filter_list(conf_file.path_second_filter, SECOND_FILTER) == -1) { //second_filter
        CMN_LOG(MY_LOG_DEBUG, "read second_fileter list failed!\n");
        exit(1);
    }
    // printf("the second is %s, the number is %d\n",second_filter[0].string,now_count_second_filter);
    if (read_filter_list(conf_file.path_black, BLACK_FILTER) == -1) { //black filter
        CMN_LOG(MY_LOG_DEBUG, "read path_black  list failed!\n");
        exit(1);
    }
    // printf("the black is %s, the number is %d\n",black_rule[0].string, now_count_black);
    if (read_filter_list(conf_file.path_white, WHITE_FILTER) == -1) { //white filter
        CMN_LOG(MY_LOG_DEBUG, "read path_white  list failed!\n");
        exit(1);
    }
    // printf("the white is %s, the number is %d\n",white_rule[0].string, now_count_white);
    if (read_src_ip(conf_file.path_src_ip_white, SRC_IP_WHITE) == -1) { //white filter
        CMN_LOG(MY_LOG_DEBUG, "read src_ip_white  list failed!\n");
        return 0;
    }
    if (read_src_ip(conf_file.path_src_ip_black, SRC_IP_BLACK) == -1) { //white filter
        CMN_LOG(MY_LOG_DEBUG, "read src_ip_white  list failed!\n");
        return 0;
    }
    if (read_http_filter_list(conf_file.path_http_white_list, HTTP_WHITE_FILTER)
            == -1) {
        CMN_LOG(MY_LOG_DEBUG, "read http_white_list  list failed!\n");
        return 0;
    }
    if (read_http_filter_list(conf_file.path_http_black_list, HTTP_BLACK_FILTER)
            == -1) {
        CMN_LOG(MY_LOG_DEBUG, "read http_black_list  list failed!\n");
        return 0;
    }
    if (read_server_ip(conf_file.path_server_ip) == -1) {
        CMN_LOG(MY_LOG_DEBUG, "read server_ip list failed!\n");
        return 0;
    }
    if (read_keywords_filter_list(conf_file.path_keywords_filter_list) == -1) {
        CMN_LOG(MY_LOG_DEBUG, "read keywords  list failed!\n");
        return 0;
    }
#endif
    //printf("the src white is %s, the number is %d\n",src_ip_white[0].string, now_count_ip_white);
    //printf("the src black is %s, the number is %d\n",src_ip_black[0].string, now_count_ip_black);
    //sprintf(str,"tcp dst port %s and host 101.71.74.235 and tcp[tcpflags]&(tcp-push)!=0",port);
    if (getcwd(CURR_PATH, 1024) == NULL) {
        CMN_LOG(MY_LOG_DEBUG, "getcwd failed!");
        exit(1);
    }
    if (CURR_PATH[strlen(CURR_PATH) - 1] != '/')
        strcat(CURR_PATH, "/");
    memset(LOG_PATH, 0, 1024);
    strcpy(LOG_PATH, CURR_PATH);
    strcat(LOG_PATH, "out_log.txt");
    fp_out = fopen(LOG_PATH, "a+");
    if (fp_out == NULL) {
        CMN_LOG(MY_LOG_DEBUG, "out_log err");
        exit(1);
    }
#if 1
    CMN_LOG(MY_LOG_DEBUG, "daemon start!\n");
    //my_daemon();
    CMN_LOG(MY_LOG_DEBUG, "daemon end!\n");
    CMN_LOG(MY_LOG_DEBUG, "syslog_init start!\n");
    syslog_init();
    CMN_LOG(MY_LOG_DEBUG, "syslog_init end!\n");
    int selectable_dns_fd = -1;
    int selectable_http_fd = -1;
    int nLoopNum = (function_choose == 3) ? 2 : 1;
    pthread_t dns_pid[MAX_DNS_THREAD_NUM] = { -1 };
    pthread_t http_pid[MAX_HTTP_THREAD_NUM] = { -1 };
    pthread_t dns_poll_pid = -1;
    pthread_t http_poll_pid = -1;
    int i = 0;
    CAPTURE_POLL stDnsPoll;
    CAPTURE_POLL stHttpPoll;
    g_apstQueuControl[DNS_PCAP_HANDLE] = new_queue();  //0
    g_apstQueuControl[HTTP_PCAP_HANDLE] = new_queue(); //1
    THREAD_PARAM stDnsThreadParam = { g_apstQueuControl[DNS_PCAP_HANDLE],
        callback[DNS_PCAP_HANDLE], DNS_PCAP_HANDLE };
    THREAD_PARAM stHttpThreadParam = { g_apstQueuControl[HTTP_PCAP_HANDLE],
        callback[HTTP_PCAP_HANDLE], HTTP_PCAP_HANDLE };
    signal(SIGTERM, sigroutine); 
    if (function_choose == 1 || function_choose == 3) {
        sprintf(str_dns, "dst port 53");
        //  printf("the dev is %s\n",dev);
        if ((cp[DNS_PCAP_HANDLE] = pcap_filter_ready(dev, str_dns, errbuf))
                == NULL) {
            CMN_LOG(MY_LOG_DEBUG, "pcap_filter_ready failed!\n");
            exit(1);
        }
        selectable_dns_fd = pcap_get_selectable_fd(cp[DNS_PCAP_HANDLE]);
        if (selectable_dns_fd == -1) {
            CMN_LOG(MY_LOG_DEBUG, "cannot use select or poll\n");
        }
        CMN_LOG(MY_LOG_DEBUG, "str_dns = %s\ndns_fd = %d\n", str_dns,
                selectable_dns_fd);
        stDnsPoll.nFd = selectable_dns_fd;
        stDnsPoll.nMonitorType = DNS_PCAP_HANDLE;
        stDnsPoll.pstPcap = cp[DNS_PCAP_HANDLE];
        pthread_create(&dns_poll_pid, NULL, capture_poll_thread,
                (void *) &stDnsPoll);
        /*start dns process*/
        for (i = 0; i < dns_thread_num; ++i) {
            pthread_create(&dns_pid[i], NULL, process_packet_thread,
                    (void *) &stDnsThreadParam);
        }
    }
    if (function_choose == 2 || function_choose == 3) {
        sprintf(str_http, "tcp dst port 80 and tcp[tcpflags]&(tcp-push)!=0");
        //  printf("the dev is %s,the port is %s\n",dev, port);
        if ((cp[HTTP_PCAP_HANDLE] = pcap_filter_ready(dev, str_http, errbuf))
                == NULL) {
            CMN_LOG(MY_LOG_DEBUG, "pcap_filter_ready failed!\n");
            exit(1);
        }
        //		//nonblock
        //
        //		if (dononblock) {
        //
        //			if (pcap_setnonblock(cp[HTTP_PCAP_HANDLE], 1, errbuf) == -1) {
        //				error("pcap_setnonblock failed: %s", errbuf);
        //			}
        //		}
        selectable_http_fd = pcap_get_selectable_fd(cp[HTTP_PCAP_HANDLE]);
        if (selectable_http_fd == -1) {
            CMN_LOG(MY_LOG_DEBUG, "cannot use select or poll\n");
        }
        CMN_LOG(MY_LOG_DEBUG, "str_http = %s\nhttp_fd = %d\n", str_http,
                selectable_http_fd);
        stHttpPoll.nFd = selectable_http_fd;
        stHttpPoll.nMonitorType = HTTP_PCAP_HANDLE;
        stHttpPoll.pstPcap = cp[HTTP_PCAP_HANDLE];
        pthread_create(&http_poll_pid, NULL, capture_poll_thread,
                (void *) &stHttpPoll);
        for (i = 0; i < http_thread_num; ++i) {
            /*start http process*/
            pthread_create(&http_pid[i], NULL, process_packet_thread,
                    (void *) &stHttpThreadParam);
        }
    }
#endif
#if 0
    //if(pcap_loop(cp , -1, callback, (u_char *)&user ) == -1)
    if(pcap_dispatch(cp , -1, callback, (u_char *)&user ) == -1)
    {
        printf("%s\n",pcap_geterr(cp));
        return 0;
    }
#endif
#if 0
#if 0
    for (;;)
    {
        struct pollfd fd[2];
        int polltimeout = -1;
        if (function_choose == 3)
        {
            fd[DNS_PCAP_HANDLE].fd = selectable_dns_fd;
            fd[DNS_PCAP_HANDLE].events = POLLIN;
            fd[HTTP_PCAP_HANDLE].fd = selectable_http_fd;
            fd[HTTP_PCAP_HANDLE].events = POLLIN;
        }
        else
        {
            fd[DNS_PCAP_HANDLE].fd = (selectable_dns_fd == -1) ? selectable_http_fd : selectable_dns_fd;
            fd[DNS_PCAP_HANDLE].events = POLLIN;
            callback[DNS_PCAP_HANDLE] = (selectable_dns_fd == -1) ? http_handler : dns_handler;
            cp[0] = (selectable_dns_fd == -1) ? cp[1] : cp[0];
        }
        int status = -1;
        status = poll(fd, nLoopNum, polltimeout);
        if (status == -1) {
            printf("Poll returns error (%s)\n",
                    strerror(errno));
        }
        else
        {
            if (status == 0)
                printf("Poll timed out\n");
            else
            {
                int i = 0;
                for (; i < nLoopNum; i++)
                {
                    if (fd[i].revents & POLLIN)
                    {
                        pcap_dispatch(cp[i] , -1, callback[i], NULL);
                    }
                }
            }
        }
    }
#endif
#if 0
    while(1)
    {
        pcap_dispatch(cp , -1, callback, (u_char *)&user );
    }
#endif
#endif
    if (function_choose == 1 || function_choose == 3) {
        for (i = 0; i < dns_thread_num; ++i) {
            pthread_join(dns_pid[i], NULL);
        }
        pthread_join(dns_poll_pid, NULL);
    }
    if (function_choose == 2 || function_choose == 3) {
        for (i = 0; i < http_thread_num; ++i) {
            pthread_join(http_pid[i], NULL);
        }
        pthread_join(http_poll_pid, NULL);
    }
    destroy_queue(&g_apstQueuControl[DNS_PCAP_HANDLE]);
    destroy_queue(&g_apstQueuControl[HTTP_PCAP_HANDLE]);
    /***********close dev*******/
    //free the internal memory
    int j = 0;
    for (; j < 2; j++) {
        if (cp[j] != NULL) {
            pcap_close(cp[j]);
            cp[j] = NULL;
        }
    }
    closelog();
    free(white_rule);
    free(black_rule);
    free(second_filter);
    free(src_ip_black);
    free(src_ip_white);
    free(http_black_list);
    free(http_white_list);
    return 0;
} 
