#include "prot.h"
#include <ctype.h>
#include "cmn_log.h"

#define NAME_LEN    100
#define DNS_LEN     1024
#define UDP_HEADER  8
#define DNS_HEADER  12




//ͨ����������ַ��׵�ַ����ȡ��Ҫ����������ͬʱ��ȡ�����������ͺ��ࡣ
int show_question_name(const char *string , char *name ,const int pay_size , u_short *type, u_short *class)
{
    int i = 0;
    int j = 0;
    const char *start = string;
    char *s = name;
    char count = 0;



    while(*string != 0)
    {
        count = *string; 
        for(i = 0; i < count; i++)
        {
            j++;
            string++;
            *name = *string;
            name++;
        }
        *name++ = '.';
        string++;
    }

    if(*string++ == 0)
        *--name = '\0'; 



    *type = ntohs(*(u_short *)string);
    string = string + 2;//sizeof(u_short) == 2 
    *class = ntohs(*(u_short *)string);

    return 0;;//record the length of name
}

/*
 * dns_respond  is dns respond body,dns is dns respond header
 *get_inquire is the inquired body,combined the all messaget into dns_respond
 */



int  input_message_dns_respond(struct dns_respond *send_respond , struct dns_inquire *get_inquire ,char *dns_respond , const struct sniff_dns *dns, const char *payload)
{



    int dns_parse_name = 0;
    int dns_parse_ip = 0;

    char *start = dns_respond;

    struct sniff_dns dns_header;

    memset(&dns_header, 0 , sizeof(struct sniff_dns));





    dns_header.dns_id = dns->dns_id;
    dns_header.dns_flag = 0x8081;//big endian
    dns_header.dns_ques =0x0100;
    dns_header.dns_ans = 0x0100;
    dns_header.dns_auth = 0x0000;
    dns_header.dns_add = 0x0000 ;

#if 0//testing

    send_respond->ip_flag = 0x0cc0;//use wireshar to capture the packet ,see the flag have the value:0xc00c,but why??

    send_respond->res_type = 0x0100; //name

    send_respond->res_class =  0x0100; //normal is 0x1, address inquire

    send_respond->time_t_live_s = 0xaff00000; //set the effictive  time of second  

    send_respond->date_length = 0x400;//cache_ip have four bytes

#endif


    //parse_name->res.inquire_ip = get_inquire->inquire_ip;

    // dns_parse_name = strlen(get_inquire->inquire_ip);
    //dns_parse_ip = strlen(parse_name->respond_ip);

    //printf("the dns_parse_name is %d\n",dns_parse_name);
    //printf("the dns_parse_ip is %d\n",dns_parse_ip);

    //dns header
    memcpy(start, (char *)&dns_header, DNS_HEADER); 

    start = start + DNS_HEADER;


    //end

    //queries
    memcpy(start, payload,get_inquire->inquire_ip_len + 4);

    start = start + get_inquire->inquire_ip_len + 4;


    //memcpy(start , (char *)&get_inquire->inq_type, 4 );//inquire type and class
    //start = start + 4;
    //end


    //answer


    //len = sizeof(struct dns_respond);



#if 0//testing
    memcpy(start,(char *)&send_respond->ip_flag, 4);//flag + type
    start = start + 4;

    memcpy(start , (char *)&send_respond->res_class, 2);
    start = start + 2;

    memcpy(start, (char *)&send_respond->time_t_live_s, 4);
    start = start + 4;

    memcpy(start , (char *)&send_respond->date_length, 2);
    start = start + 2;

    memcpy(start, (char *)&send_respond->respond_ip, 4);
    start = start + 4;


#endif
#if 1
    memcpy(start, dns_content,16);
    start = start + 16;
#endif

    *start = '\0';



    return 16 + get_inquire->inquire_ip_len + 4 + DNS_HEADER;

    //end

    //   printf("dns_id is %x, dns_flag is %x, dns_ques is %x , dns_ans is %x , dns_auth is %x ,dns_add is %x\n",dns_header.dns_id, dns_header.dns_flag, \
    //  dns_header.dns_ques, dns_header.dns_ans,dns_header.dns_auth,dns_header.dns_add);
    //just one packet!

}


int dns_regex_compare(int method, const char *http)
{
    char rule[100] = {0};
    int i = 0;

    if(method == 0){

        for(i = 0; i < now_count_black; i++)
        {
            // printf("the file host is %s\n",file_host);
            //if(regex_compare(http, black_rule[i].string) >= 0)
            if(str_compare(http, black_rule[i].string) >=  0)
            {
                return 0;
            }

        }
        return -1;

    }


    if(method == 1){
        for(i = 0; i < now_count_white; i++)
        {  
            // printf("the file host is %s\n",file_host);

            //if(regex_compare(http, white_rule[i].string) >= 0)
            if(str_compare(http, white_rule[i].string) >=  0)
            {  
                return 0;
            }

        }    
        return -1;


    }

}



void  dns_handler(u_char *user, const struct pcap_pkthdr* h, const u_char *bytes)
{


    struct sniff_ip *ip;
    struct sniff_tcp *tcp;
    struct sniff_ethernet *eth;
    struct sniff_udp *udp;
    struct sniff_dns *dns;
    struct dns_inquire *ques;
    char name[NAME_LEN] = {0}; //inquire name
    const u_char *payload = NULL;
    int pay_size = 0;

    char cache_ip[100] = {0};//cache server ip

    /*libnet ready*/
    libnet_t *e = NULL;
    char errbuf[BUFFER]={0};
    libnet_ptag_t udp_tag,ip_tag,eth_tag;
    u_int payload_ip_size = 0;


    struct dns_inquire get_inquire;
    struct dns_respond send_respond;
    char dns_res_mess[DNS_LEN]={0};
    int dns_len = 0;
    // struct in_addr_t addr1;


    int i =0;
    int j = 0;





    u_long src_ip = 0;
    u_long dst_ip = 0;
    u_char enet_dst[6]={0};
    u_char enet_src[6]={0};
    u_short udp_sprt = 0;
    u_short udp_dprt = 0;
    u_long match_ip = 0;



    struct in_addr s_ip;
    char s_ip_buf[20] = {0};
    char sys_time[40] ={0};



    memset(&get_inquire, 0, sizeof(struct dns_inquire));
    memset(&send_respond, 0, sizeof(struct dns_respond));











    //��ȡЭ��ͷ��Ϣ,��λ�������ݵ�ַ
    eth = (struct sniff_ethernet *)(bytes);
    ip  =  (struct sniff_ip *)(bytes + ETHER_HEAD);
    udp = (struct sniff_udp *)(bytes + ETHER_HEAD + sizeof(struct sniff_ip));
    dns = (struct sniff_dns *)(bytes + ETHER_HEAD + sizeof(struct sniff_ip) + sizeof(struct sniff_udp)); 
    payload = (u_char *)(bytes + sizeof(struct sniff_dns) + ETHER_HEAD + sizeof(struct sniff_ip) + sizeof(struct sniff_udp)); 
    pay_size = ntohs(udp->udp_len) - sizeof(struct sniff_udp) - sizeof(struct sniff_dns);

    match_ip = *(u_long *)(ip->ip_src);
    s_ip.s_addr = match_ip;
    strcpy(s_ip_buf, inet_ntoa(s_ip));




    if(dns->dns_flag != 0x0001)//dns����Ǳ�׼����0x0001�����˳�
        return ;



    //�ڰ���Դ�ɣе�ƥ��
#if 1

    if(now_count_ip_black  != 0){
        for(i = 0; i < now_count_ip_black; i++){
            if(compare_ip(src_ip_black[i].string,(u_int)match_ip) == 0)
                return;
        }
    }


    if(now_count_ip_white != 0){
        for(j = 0; j < now_count_ip_white; j++){
            if(compare_ip(src_ip_white[j].string,(u_int)match_ip) != 0)
                continue ;
            else
                break;
        }

        if(j == now_count_ip_white)
            return;
    }



#endif







    //parse inquire
    if (show_question_name(payload, name , pay_size, &get_inquire.inq_type , &get_inquire.inq_class) != 0)
    {
        CMN_LOG(MY_LOG_DEBUG, "parse inquire name failed!\n");
        return ;
    }





    get_inquire.inquire_ip_len = strlen(name) + 2;//��������ַ�����Ϊ��ͷ�ͽ�β����һ��

    //printf("the name is %s,len is %d\n",name, get_inquire.inquire_ip_len);



    // printf("the type is %d, the class is %d\n",get_inquire.inq_type , get_inquire.inq_class);





    //  printf("the dns_respond size is %d\n",strlen(dns_res_mess));

    //�ڰ�����ж�

    if(dns_regex_compare(0, name) < 0) //black filter
    {
        if(dns_regex_compare(1, name) >= 0)//white filter
        {



            //printf("the name is %s,len is %d\n",name, get_inquire.inquire_ip_len);




            //ketama_select_server(cache_ip, path_server_ip, NULL, name);

            srand((unsigned)time(NULL));


            strcpy(cache_ip,server_ip[rand()%now_count_server_ip].string);

            //send_respond.respond_ip = inet_addr("192.168.90.21"); 

            send_respond.respond_ip = inet_addr(cache_ip); 

            //printf("the send respond ip is %x\n",send_respond.respond_ip);

            dns_content[3] = send_respond.respond_ip;







            //build respond
            dns_len = input_message_dns_respond(&send_respond , &get_inquire, dns_res_mess, dns ,payload ); 

            // printf("parse_name message is %s,type is %x,class is %x, time_to_live is %x,data_length is %x, the parse is %s\n",\
            parse_name.res.inquire_ip,parse_name.res.inq_type,parse_name.res.inq_class,parse_name.time_t_live_s, parse_name.date_length, parse_name.respond_ip);


            /*
             *build package
             */

#if 1


            e = libnet_init(
                    LIBNET_LINK,
                    dev_name,
                    errbuf
                    );

            if(e == NULL)
            {
                fprintf(stderr, "libnet_init() for dns faild!: %s", errbuf);
                exit(1);
            }

            //build udp

            udp_sprt  = ntohs(udp->udp_dport);
            udp_dprt  = ntohs(udp->udp_sport);

            udp_tag = libnet_build_udp( 
                    udp_sprt,
                    udp_dprt,
                    UDP_HEADER+ dns_len,
                    0,
                    dns_res_mess,
                    dns_len,
                    e,
                    0
                    );


            if(udp_tag == -1){
                CMN_LOG(MY_LOG_DEBUG, "libnet_build_udp failure!\n ");
                exit(1);
            }

            //build ipv4
            src_ip = *(u_long *)(ip->ip_dst);
            dst_ip = *(u_long *)(ip->ip_src);


            ip_tag = libnet_build_ipv4(
                    LIBNET_IPV4_H + UDP_HEADER + dns_len,
                    0,
                    0x33,
                    0,
                    200,
                    IPPROTO_UDP,
                    0,
                    src_ip,
                    dst_ip,
                    NULL,
                    0,
                    e,
                    0
                    );

            if(ip_tag == -1)
            {
                CMN_LOG(MY_LOG_DEBUG, "libnet_build_ipv4 failure!\n");
                exit(1);
            }




            // memcpy(enet_dst, eth->ether_shost, 6);
            //memcpy(enet_src, eth->ether_dhost, 6);


            eth_tag = libnet_build_ethernet(
                    eth->ether_shost,
                    eth->ether_dhost,
                    ETHERTYPE_IP,
                    NULL,
                    0,
                    e,
                    0
                    );

            if(eth_tag == -1)
            {
                CMN_LOG(MY_LOG_DEBUG, "libnet_build_eth failure!\n");
                exit(1);
            }



            pay_size = libnet_write(e);
            if(pay_size == -1)
            {
                CMN_LOG(MY_LOG_DEBUG, "libnet_write error!\n");
                exit(1);
            }
            //fprintf(stderr, "dns %s ,write %d bytes UDP packet!\n",name , pay_size);

            libnet_destroy(e);

#endif
            get_sys_time(sys_time);
            fprintf(fp_out, "%s %s %02d %s %s %s\n","I",sys_time,1, s_ip_buf,name, cache_ip);
            //CMN_LOG(MY_LOG_DEBUG, "%s %s %02d %s %s %s\n","I",sys_time,1, s_ip_buf,name, cache_ip);
            fflush(fp_out);

        }//endof white handle
    }//endof black handle

}




