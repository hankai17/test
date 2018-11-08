#ifndef __CACHE__
#define __CACHE__
#include <pcap.h>
#include <sys/prctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pcap/pcap.h>
#include <libnet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <regex.h>
//#include <ketama.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>
#endif



#ifndef __PROTO__
#define __PROTO__

#define ETHER_ADDR_LENGH 6
#define BUFFER 2048
#define ETHER_HEAD 14
#define LEN     1024

struct sniff_ethernet {
u_char ether_dhost[ETHER_ADDR_LENGH]; /* ĿµÄ÷ĵØ· */
u_char ether_shost[ETHER_ADDR_LENGH]; /* ԴÖ»úַ */
u_short ether_type; /* IP? ARP? RARP? etc */
};



struct sniff_ip {
#if BYTE_ORDER == LITTLE_ENDIAN
u_int ip_hl:4, /* ͷ²¿³¤¶È**/
ip_v:4; /* °汾ºÅ**/
#if BYTE_ORDER == BIG_ENDIAN
u_int ip_v:4, /* °汾ºÅ**/
ip_hl:4; 
#endif
#endif /* not _IP_VHL */
u_char ip_tos; /* ·þÎµÄà */
u_short ip_len; /* ×³¤¶È**/
u_short ip_id; /*°üÅ**/
u_short ip_off; /* ËƬƫÒ */
#define IP_RF 0x8000 /* ±£ÁµÄé±ê */
#define IP_DF 0x4000 /* dont fragment flag */
#define IP_MF 0x2000 /* ¶àƬ±ê*/
#define IP_OFFMASK 0x1fff /*·ֶÎ» */
u_char ip_ttl; /* Ê¾ݰüæ¼ä*/
u_char ip_p; /* ËʹÓµÄ­Ò */
u_short ip_sum; /* УÑºÍ*/
//struct in_addr ip_src,ip_dst; /* ԴµØ·¡¢ĿµĵØ·*/
u_char ip_src[4];
u_char ip_dst[4];
};

struct sniff_tcp {
u_short th_sport; /* Դ¶˿Ú*/
u_short th_dport; /* ĿµĶ˿Ú*/
u_int th_seq; /* °ü*/
u_int th_ack; /* ȷÈÐºÅ*/
#if 0
#if BYTE_ORDER == LITTLE_ENDIAN
u_int th_x2:4, /* »¹ûÓÓµ½ */
th_off:4; /* Ê¾Ý«Ò */
#endif
#if BYTE_ORDER == BIG_ENDIAN
u_int th_off:4, /* Ê¾Ý«Ò*/
th_x2:4; /*»¹ûÓÓµ½ */
#endif
#endif

#if 0
u_char th_offx2;
#define TH_OFF(th) (((th)->th_offx2 & 0xf0)>>4)
#endif

#if (LIBNET_LIL_ENDIAN)
u_int8_t th_x2:4,         /* (unused) */
th_off:4;        /* data offset */
#endif
#if (LIBNET_BIG_ENDIAN)
u_int8_t th_off:4,        /* data offset */
th_x2:4;         /* (unused) */
#endif



u_char th_flags;
#define TH_FIN 0x01
#define TH_SYN 0x02
#define TH_RST 0x04
#define TH_PUSH 0x08
#define TH_ACK 0x10
#define TH_URG 0x20
#define TH_ECE 0x40
#define TH_CWR 0x80
#define TH_FLAGS (TH_FINTH_SYNTH_RSTTH_ACKTH_URGTH_ECETH_CWR)
u_short th_win; /* TCP»¬¶¯´°¿Ú*/
u_short th_sum; /* ͷ²¿УÑºÍ*/
u_short th_urp; /* ½����þÎλ */
};


struct sniff_udp
{
    u_short udp_sport;
    u_short udp_dport;
    u_short udp_len;
    u_short udp_sum;
};

struct sniff_dns
{
    u_short dns_id;
    u_short dns_flag;
    u_short dns_ques;
    u_short dns_ans;
    u_short dns_auth;
    u_short dns_add;
};

struct dns_inquire
{
    u_int  inquire_ip_len;//record the ip len
    u_short inq_type;
    u_short inq_class;
};




struct dns_respond
{
    u_short ip_flag; 
    u_short res_type;
    u_short res_class;
    u_int time_t_live_s;
    u_short date_length;
    u_int respond_ip;
};




#endif



struct conf_value{
    char port[12];
    char dev[12];
    char path_white[60];
    char path_black[60];
    char path_server_ip[60];
    char path_second_filter[60];
    char path_src_ip_white[60];
    char path_src_ip_black[60];
    char function[4];
    char path_http_white_list[60];
    char path_http_black_list[60];
};

typedef struct{
    char string[100];
}rule;

#define SECOND_FILTER    0
#define WHITE_FILTER     1
#define BLACK_FILTER     2


#define SRC_IP_BLACK    0
#define SRC_IP_WHITE    1


#define HTTP_BLACK_FILTER 0
#define HTTP_WHITE_FILTER 1

extern rule *white_rule;
extern rule *black_rule;
extern rule *second_filter;
extern rule *src_ip_white;
extern rule *src_ip_black;
extern rule *http_white_list;
extern rule *http_black_list;

extern int now_count_white,max_count_white;
extern int now_count_black,max_count_black;
extern int now_count_second_filter,max_count_second_filter;
extern int now_count_ip_white,max_count_ip_white;
extern int now_count_ip_black,max_count_ip_black;
extern int now_count_http_white_list,max_count_http_white_list;
extern int now_count_http_black_list,max_count_http_black_list;

#define NEW(t,n)             ((t*)(malloc((sizeof(t))*(n))))
#define RENEW(o,t,n)        (t*)(realloc((void *)o, (sizeof(t))*(n)))

#define CF      struct conf_value
extern char dev_name[60];
extern char path_server_ip[60];

extern int dns_content[4];

extern FILE *fp_dns;
extern FILE *fp_http;


#ifndef __CACHE_FUNCTION__
#define __CACHE_FUNCTION__


extern char *strcpy_head(char *dst, const char *src, const short number, const char *flag); 
extern void cancel_char(char *string);
extern int file_compare_http_host(const char *http_host , const int handle_method);
typedef void (*pacp_handler)(u_char *user, const struct pcap_pkthdr* h, const u_char *bytes);
extern void dns_handler(u_char *user, const struct pcap_pkthdr* h, const u_char *bytes);
//extern int ketama_select_server(char *ip, char *filename,char *url, const char *http_host);
extern void *pth_handle_dns(void  *);
extern int read_filter_list(char *filename ,const int method);
extern int compare_ip(const char *range, const u_int ip);//range the range of ip
extern int read_src_ip(char *filename, const int method);
extern int read_http_filter_list(char *filename, const int method);
extern void http_handler(u_char *user, const struct pcap_pkthdr* h, const u_char *bytes);


extern void get_sys_time(char *buf);
extern int str_compare(const char *http, const char *parse);
extern int str_compare_http_host(const char *http, const int handle_method);
extern void my_daemon(void);



extern void init_proc_title(int argc, char *argv[]); 
extern void set_proc_title(const char *name);
extern void get_proc_name(char *name);



#endif






























