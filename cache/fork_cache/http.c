#include "prot.h"
#define BLACK_HANDLE 1
#define WHITE_HANDLE 0
void http_handler(u_char *user, const struct pcap_pkthdr *h,const u_char *bytes)
{
    struct sniff_ip *ip;
    struct sniff_tcp *tcp;
    struct sniff_ethernet *eth;
    short i  = 0;
    short j = 0;
    int pmatch_eo = 0;
    char *point = NULL;
    char errbuf[BUFFER]= {0};
    char white_host[100]= {0};
    char http_host[100]={0};
    char request_line[4096]={0};
    int add_dir_host = 0;
    char send_message[BUFFER] = {0}; 
    char request[BUFFER] = {0};
    char cache_ip[100] ={0};
    char whether_send = 0;
    char url[BUFFER] = {0};
    char file_name[100] = {0};
    u_long match_ip = 0;
    char sys_time[40] ={0};
    struct in_addr s_ip;
    char s_ip_buf[20] = {0};
    eth = (struct sniff_ethernet *)(bytes);
    ip =  (struct sniff_ip *)(bytes + ETHER_HEAD);
    tcp = (struct sniff_tcp *)(bytes + ETHER_HEAD + sizeof(struct sniff_ip));
    //指针指向各协议的首地址
#if 0
    printf("the src eth is %x,%x,%x,%x,%x,%x\n",eth->ether_shost[0],eth->ether_shost[1],eth->ether_shost[2],eth->ether_shost[3],\
            eth->ether_shost[4],eth->ether_shost[5]);
    //printf("hello world! %x , %x,the flag is %x\n",ntohs(tcp->th_dport),ntohs(tcp->th_sport), tcp->th_flags);
    printf("hello world! %x , %x,the flag is %x\n",tcp->th_dport,tcp->th_sport, tcp->th_flags);
    printf("the dst_ip is %x\n",*(u_long *)(ip->ip_dst));
    printf("the src_ip is %x\n",*(u_long *)ip->ip_src);
    printf("the seq is %x,the ack is %x\n",tcp->th_seq,tcp->th_ack);
#endif
    //src_ip_white and src_ip_black 
    match_ip = *(u_long *)(ip->ip_src);
    s_ip.s_addr = match_ip;
    strcpy(s_ip_buf, inet_ntoa(s_ip));
    if(now_count_ip_black  != 0){
        for(i = 0; i < now_count_ip_black; i++){
            if(compare_ip(src_ip_black[i].string,(int)match_ip) == 0)
                return;
        }
    }
    if(now_count_ip_white != 0){
        for(j = 0; j < now_count_ip_white; j++){
            if(compare_ip(src_ip_white[j].string,(int)match_ip) != 0)
                continue ;
            else
                break;
        }
        if(j == now_count_ip_white)
            return;
    }
    i = ntohs(ip->ip_len) - 0x28;
    point = (char *)tcp;
    point= point + 20;
    memset(http_host , 0 ,100);
    //获取请求行
    if(strcpy_head(request_line, point, i ,"/") == NULL)
    {
        //printf("get request line failed!\n");
        return;
    }

    printf("the requst_line=%s\n", request_line);

    //删除请求头中的http版本
    if(delete_http_version(request_line) == -1)
        return;
    printf("the second request_line=%s\n",request_line);
    if(strlen(request_line) == 0)
        return;
    //获取请求头中的最终文件名,然后检查是否符合要求,如果属于*.文件，则继续执行，否则不处理，并返回
    if(pre_file_name_check(file_name, request_line , strlen(request_line)) == -1 )
        return ;
    printf("file_name=%s\n",file_name);
    //获取host
    if(strcpy_head(http_host, point, i ,"Host: ") == NULL)
    {
        //printf("not found the http header!\n");
        return ;
    }
    printf("the HTTP host=%s\n",http_host); 
    strcat(request, "File: ");
    strcat(request, file_name);
    strcat(request,"\r\nHost: ");
    strcat(request,http_host);
       printf("the parse request=%s\n",request);
    if(file_compare_http_host(request, BLACK_HANDLE) < 0)
    {
        if((pmatch_eo = file_compare_http_host(request , WHITE_HANDLE)) >= 0)
        {
            /*ketama_select_server is used to choose cache_ip ,according to the url
             *the assemble_url is used  to get the url message
             */
            //截取“？”字符前的信息，并根据ｈｏｓｔ信息，判断是否添加ｈｏｓｔ目录，组成新的ｕｒｌ。 
            //如果返回１，说明ｈｏｓｔ是ｉｐ，否则ｈｏｓｔ是域名
            //1. add_dir_host =  assemble_url(url,request_line,http_host);
            //       printf("the url is %s\n",url);
            //根据ｕｒｌ和ｈｏｓｔ进行一致性哈希的计算，结果放入cache_ip中 
            //  2. ketama_select_server(cache_ip,path_server_ip,url,http_host);
            strcpy(cache_ip,"192.168.90.22");
            //    printf("before request_line is %s\n",request_line);
            //组成ｈｔｔｐ信息发送给客户端
            // if(add_dir_host == 1) //the http host is ip
            // sprintf(send_message,"HTTP/1.1 302 Found\r\nLocation: http://%s/%s\r\n\r\n",cache_ip,http_host,request_line); 
            // else
            sprintf(send_message,"HTTP/1.1 302 Found\r\nLocation: http://%s/%s/%s/%s\r\n\r\n",cache_ip,http_host,"myjizhong",request_line); 
            printf("send_message=%s\n",send_message);
            get_sys_time(sys_time);
            fprintf(fp_http, "%s %s %02d %s http://%s/%s http://%s/%s/%s/%s\n","I",sys_time,2 ,s_ip_buf,  http_host,  request_line,  cache_ip,  http_host, "myjizhong", request_line);
            printf("fp_http=%s\n",send_message);
            fflush(fp_http);
#if 1
            /*
             *create the parameter 
             */
            u_long src_ip = 0;
            u_long dst_ip = 0;
            u_short src_prt = 0;
            u_short dst_prt = 0;
            char *payload = NULL;
            u_short payload_s = 0;
            libnet_ptag_t t;
            libnet_t *e = NULL;
            u_char enet_dst[6];
            u_char enet_src[6];
            u_int ack;
            /*
             *combine the packet for sending 
             */
            e = libnet_init(
                    LIBNET_LINK,
                    dev_name,
                    errbuf
                    );
            if(e == NULL)
            {
                fprintf(stderr,"libnet_init() failed : %s", errbuf);
                exit(1);
            }
#if 0
            t = libnet_build_tcp_options(
                    "\003\003\012\001\002\004\001\011\010\012\077\077\077\077\000\000\000\000\000\000",
                    0x14,
                    e,
                    0);
            if(t == -1)
            {
                fprintf(stderr, "Can't built TCP option: %s\n",libnet_geterror(e));
                exit(1);
            }
#endif
            /*set tcp header value*/
            payload = send_message;
            payload_s = strlen(payload);
            src_prt = ntohs(tcp->th_dport);
            dst_prt = ntohs(tcp->th_sport);
            ack =ntohl(tcp->th_seq) + ntohs(ip->ip_len) - 0x28 - 12 ;//why add 12??
            t = libnet_build_tcp(
                    src_prt,
                    dst_prt,
                    ntohl(tcp->th_ack),   /*sequence number*/
                    ack,   /*acknowlegement number */
                    //  TH_SYN,        /*control flags */
                    TH_ACK + TH_PUSH,
                    32767,          /*windows size*/
                    0,              /*checksum*/
                    0,             /*urgent pointer*/
                    LIBNET_TCP_H  +  payload_s,  /*TCP PACKET SIZE*/
                    payload,
                    payload_s,
                    e,
                    0);
            if(t == -1)
            {
                fprintf(stderr, "Can't not build TCP header:%s\n",libnet_geterror(e));
                exit(1);
            }
            /*set ip header value*/
            src_ip = *(u_long *)(ip->ip_dst); 
            dst_ip = *(u_long *)(ip->ip_src);
            t = libnet_build_ipv4(
                    LIBNET_IPV4_H + LIBNET_TCP_H  + payload_s, /*length*/
                    0,      /*TOS*/
                    242,    /*IP ID*/
                    0x00,      /*IP Frag*/
                    64,        /*TTL*/
                    IPPROTO_TCP, /*PROTOCAL*/
                    0,                                          /* checksum */
                    src_ip,                                     /* source IP */
                    dst_ip,                                     /* destination IP */
                    NULL,                                       /* payload */
                    0,                                          /* payload size */
                    e,                                          /* libnet handle */
                    0);                                         /* libnet id */
            if(t == -1)
            {
                fprintf(stderr, "Can't build IP header: %s\n",libnet_geterror(e));
                exit(1);
            }
            /*write data int ether*/
            //    memcpy(enet_dst,eth->ether_shost, 6 );
            //  memcpy(enet_src, eth->ether_dhost, 6);
            t = libnet_build_ethernet(
                    eth->ether_shost,
                    eth->ether_dhost,
                    ETHERTYPE_IP,
                    NULL,       /*payload */
                    0,          /*payload size*/
                    e,          /*libnet handle*/
                    0);         /*libnet id*/
            if(t == -1)
            {
                fprintf(stderr, "Can't build ether header :%s\n", libnet_geterror(e));
                exit(1);
            }
            /*write it */
            int c = 0;
            c = libnet_write(e);
            if(c  == -1)
            {
                fprintf(stderr, "Write error: %s\n", libnet_geterror(e));
                exit(1);
            }
            //  fprintf(stderr, "write %d byte TCP packet\n",c);
            libnet_destroy(e);
#endif
        }
    }
}
