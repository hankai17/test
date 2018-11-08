#include "prot.h"
#define LOAD_LEN   2048
char path_server_ip[60];
char dev_name[60] = {0};
FILE *fp_dns = NULL;
FILE *fp_http = NULL;
rule *white_rule;
rule *black_rule;
rule *second_filter;
rule *src_ip_white;
rule *src_ip_black;
rule *http_white_list;
rule *http_black_list;
int now_count_white = 0;
int max_count_white = 0;
int now_count_black = 0;
int max_count_black = 0;
int now_count_second_filter =0;
int max_count_second_filter = 0;
int now_count_ip_white = 0;
int max_count_ip_white = 0;
int now_count_ip_black = 0;
int max_count_ip_black = 0;
int max_count_http_white_list = 0;
int max_count_http_black_list = 0;
int now_count_http_black_list = 0;
int now_count_http_white_list = 0;
//提前对dns组包，查看速度是否提前
int dns_content[4] ={0x01000cc0,0x00000100,0x0400ee01,0x0};
pcap_t *pcap_filter_ready(const char *dev, const char *str, char *errbuff)
{
    bpf_u_int32 ipaddress,ipmask;
    struct bpf_program bp;
    pcap_t *cp = NULL;
    cp = pcap_open_live(dev, 65535, 0, 1, errbuff);//overtime  1ms
    if(cp == NULL)
    {
        perror("open failed!");
        printf("%s\n",errbuff);
    }
    else if(errbuff[0] != 0)
    {
        printf("%s\n",errbuff);
        return NULL;
    }
    if(pcap_lookupnet(dev,&ipaddress,&ipmask, errbuff ) < 0)
    {
        ipmask = 0;
        ipaddress = 0;
        perror("pcap_lookupnet failed!\n");
        return NULL;
    }
    if(pcap_compile(cp, &bp, str, 0, ipmask ) == -1)
    {
        printf("%s\n",pcap_geterr(cp));
        return NULL;
    }
    /*****set filter****/
    if(pcap_setfilter(cp,&bp) == -1)
    {
        printf("%s\n",pcap_geterr(cp));
        return NULL;
    }
    return cp;
}
int main(int argc, char *argv[])
{
    int function_choose = 0;
    char CURR_PATH[1024] = {0};//get current path
    char LOG_PATH[1024] = {0};
    int i = 0;
    int max = 0;
    pid_t pd = 0;
    CF conf_file;
    char str[1024] = {0};
    char errbuf[BUFFER];
    pcap_t *cp = NULL;
    char *port = NULL;
    char *dev = NULL;
    //    bpf_u_int32 ipaddress,ipmask;
    //struct bpf_program bp;
    int white_fd;
    int black_fd;
    int filter_fd;
    //char str[1024] = " tcp dst port 80 and host 101.71.74.235 and tcp[tcpflags]&(tcp-push)!=0 ";
    // char str[1024] = " tcp dst port 80 and tcp[tcpflags]&(tcp-push)!=0 ";
    memset(errbuf, 0, BUFFER);
    /*
     *init the argument :port , dev .....
     */
    if(get_conf_value("cache.conf",&conf_file) == -1)
    {
        printf("cache.conf error!\n");
        exit(1);
    }
    port = conf_file.port;
    dev = conf_file.dev;
    function_choose = atoi(conf_file.function);//get the function of choose
    strcpy(path_server_ip, conf_file.path_server_ip);
    strcpy(dev_name, dev);
    /*
     *strcpy(white_path,conf_file.path_white);
     *strcpy(black_path,conf_file.path_black);
     *strcpy(path_second_filter, conf_file.path_second_filter);
     *将文件内容导入内存当中，提高读取速度
     */
#if 1
    if(read_filter_list(conf_file.path_second_filter, SECOND_FILTER) == -1){ //second_filter
        printf("read second_fileter list failed!\n");
        return 0;
    }
    // printf("the second is %s, the number is %d\n",second_filter[0].string,now_count_second_filter);
    if(read_filter_list(conf_file.path_black, BLACK_FILTER) == -1){//black filter
        printf("read path_black  list failed!\n");
        return 0;
    }
    // printf("the black is %s, the number is %d\n",black_rule[0].string, now_count_black);
    if(read_filter_list(conf_file.path_white , WHITE_FILTER) == -1){//white filter
        printf("read path_white  list failed!\n");
        return 0;
    }
    // printf("the white is %s, the number is %d\n",white_rule[0].string, now_count_white);
    if(read_src_ip(conf_file.path_src_ip_white , SRC_IP_WHITE) == -1){//white filter
        printf("read src_ip_white  list failed!\n");
        return 0;
    }
    if(read_src_ip(conf_file.path_src_ip_black , SRC_IP_BLACK) == -1){//black filter
        printf("read src_ip_white  list failed!\n");
        return 0;
    }
    if (read_http_filter_list(conf_file.path_http_white_list, HTTP_WHITE_FILTER) == -1){
        printf("read http_white_list  list failed!\n");
        return 0;
    }
    if (read_http_filter_list(conf_file.path_http_black_list, HTTP_BLACK_FILTER) == -1){
        printf("read http_black_list  list failed!\n");
        return 0;
    }
#endif
    printf("the src white is %s, the number is %d\n",http_white_list[0].string, now_count_http_white_list);
    printf("the src black is %s, the number is %d\n",http_black_list[0].string, now_count_http_black_list);
    //sprintf(str,"tcp dst port %s and host 101.71.74.235 and tcp[tcpflags]&(tcp-push)!=0",port);
#if 0 
    if(function_choose == 1)
    {
        sprintf(str,"dst port 53");
        printf("the dev is %s\n",dev);
    }
    else if(function_choose == 2)
    {
        sprintf(str,"tcp dst port 80 and tcp[tcpflags]&(tcp-push)!=0");
        printf("the dev is %s,the port is %s\n",dev, port);
    }
    printf("%s\n",str);
#endif
    /***dispath the page*****/
    int  user;
    pcap_handler callback = NULL;
    if(getcwd(CURR_PATH,1024) == NULL){
        perror("getcwd failed!");
        exit(1);
    }
    //my_daemon();
    switch(function_choose){
        case 3: i = 1;max = 3;break;
        case 2: i = 2;max = 3;break;
        case 1: i = 1;max = 2;break;
    }
    for(; i < max ; i++){
        pd = fork();
        if(pd < 0){
            perror("fork failed\n");
            exit(1);
        }else if(pd == 0){
            if(i == 1){//dns fork
                //open dns_log.txt
                memset(LOG_PATH,0,1024);
                strcpy(LOG_PATH,CURR_PATH);
                strcat(LOG_PATH,"/dns_log.txt");
                fp_dns = fopen(LOG_PATH,"a+");
                if(fp_dns == NULL){
                    perror("dns_log err");
                    exit(1);
                }
                sprintf(str,"dst port 53");
                //pcap filter ready 
                if((cp = pcap_filter_ready(dev, str, errbuf)) == NULL){
                    fprintf(stdout,"pcap_filter_ready failed!\n");
                    exit(1);
                }
                init_proc_title(argc, argv);
                set_proc_title("dns");
                set_proc_name("dns");
                callback = dns_handler;
                while(1){
                    pcap_dispatch(cp , -1, callback, (u_char *)&user );}
                exit(1);
            }//end fork i=1
            if(i = 2){//http fork
                //open http_log.txt
                memset(LOG_PATH,0,1024);
                strcpy(LOG_PATH,CURR_PATH);
                strcat(LOG_PATH,"/http_log.txt");
                fp_http = fopen(LOG_PATH,"a+");
                if(fp_http == NULL){
                    perror("http_log err");
                    exit(1);
                }
                sprintf(str,"tcp dst port %s and tcp[tcpflags]&(tcp-push)!=0",port);
                //pcap filter ready 
                if((cp = pcap_filter_ready(dev, str, errbuf)) == NULL){
                    fprintf(stdout,"pcap_filter_ready failed!\n");
                    exit(1);
                }
                init_proc_title(argc, argv);
                set_proc_title("http");
                set_proc_name("http");
                callback = http_handler;
                while(1){
                    //printf("*\n");
                    pcap_dispatch(cp , -1, callback, (u_char *)&user );}
                //exit(1);
            }//end fork i = 2
        }//end pid =0
    }//end for() fork
    exit(0);
    /***********close dev*******/
    //free the internal memory
#if 0
    pcap_close(cp);
    free(white_rule);
    free(black_rule);
    free(second_filter);
    free(src_ip_black);
    free(src_ip_white);
    return 0;
#endif
}
