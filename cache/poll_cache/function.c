#include "prot.h"
#include "cmn_log.h"

#ifndef PR_SET_NAME
#define PR_SET_NAME 15
#endif

#ifndef PR_GET_NAME
#define PR_GET_NAME 16
#endif

static char *arg_start;
static char *arg_end;
static char *env_start;
extern char **environ;

//cal host and url ,consisten hashing
#if 0
int ketama_select_server(char *ip, char *filename,char *url, const char *http_host)
{
    
    char input[BUFFER] = {0};

    if(url == NULL)
    strcpy(input,http_host);
    else{
    strcpy(input,http_host);
    strcat(input,"/");
    strcat(input,url);
    }



    ketama_continuum c;
    ketama_roll(&c, filename);

    if(ketama_error() == NULL)
    return -1;

    unsigned int kh = ketama_hashi(input);
    mcs *m = ketama_get_server(input,c);
   // printf("the %s ,%u ,%u ,%s\n",host,kh,m->point, m->ip);

   strcpy(ip, m->ip);


    ketama_smoke(c);

    return 0;

}
    



#endif

int regex_compare(const char *http_host, const char *file_host)
{
    
    regmatch_t  pmatch;
    regex_t re; 
    char errbuff[1024] ={0};
    int err = 0;

    if( err = regcomp(&re,file_host,REG_EXTENDED))
    {
        //CMN_LOG(MY_LOG_DEBUG, "compile succeed!\n");
    }

    if(err)
    {
        regerror(err,&re,errbuff,sizeof(errbuff));
        //CMN_LOG(MY_LOG_DEBUG, "%s--error: %s\n",file_host,errbuff);
        return -1;
    }
      //   printf("Total has subexpression:%d\n", re.re_nsub);


      err = regexec(&re,http_host, 1, &pmatch ,0);

    if(err == REG_NOMATCH)
    {
        //printf("Sorry,no match.\n");
        //printf("Sorry,no match..%s\n",file_host);
        regfree(&re);
        return -1;
    }else if(err)
    {
        regerror(err, &re, errbuff,sizeof(errbuff));
        //CMN_LOG(MY_LOG_DEBUG, "error:regexec: %s\n",errbuff);
        regfree(&re);
        return -1;
    }
      //  printf("Match success!\n");
        regfree(&re);
        return pmatch.rm_eo;


}


int file_compare_http_host(const  char *http, const int handle_method , const char *request_line)
{

    char *p = NULL;
    int  pmatch_eo = 0;
    int i = 0;
    int j = 0;

    


 if(handle_method == 0)
 {
    //while((next_char = mmap_gets(rule,100,next_char)) != NULL)
    for(; i < now_count_http_white_list; i++)
    {


        //printf("the file host is %s\n",rule);


            if(regex_compare(http, http_white_list[i].string) >= 0)
             {

                    for(j = 0; j < now_count_second_filter; j++){
                        //if((pmatch_eo = regex_compare(http,second_filter[j].string)) >= 0)
                        if((pmatch_eo = str_compare(http,second_filter[j].string)) >= 0)
                         {
                             return pmatch_eo;
                         }
                    }
             }

     }

     for(i = 0; i < now_count_keywords_filter; i++ )
     {
        if(strstr(request_line,keywords_filter[i].string) != NULL){

               for(j = 0; j < now_count_second_filter; j++){
                        //if((pmatch_eo = regex_compare(http,second_filter[j].string)) >= 0)
                        if((pmatch_eo = str_compare(http,second_filter[j].string)) >= 0)
                         {
                             return pmatch_eo;
                         }
                }


        }
     
     }

     return -1;
 }

 if(handle_method == 1)
 {
   for(; i < now_count_http_black_list; i++)
   {
       //printf("the file host is %s\n",rule);
           
           if(regex_compare(http, http_black_list[i].string) >= 0)
            {  
               return pmatch_eo;
            
            }   
   }
 
    return -1;
 }

}





char *strcpy_head(char *dst,const  char *src,const short number,const char *flag)
{

   char *start = NULL;
   char *end = NULL;

    short nu = number;

    if(nu <= 0)
        return NULL;
    

    if(nu >= 2016)
        nu = 2016;
    


    if((start = strstr(src,flag)) == NULL)
        return NULL;

    if((end = strstr(start,"\r\n")) == NULL)
    {
        //printf("found the %s,but not found %s\n",flag,"\r\n");
        return NULL;
    }

    start = start + strlen(flag);
    if(end - start > nu)
        return NULL;

    memcpy(dst, start, end - start);
    //printf("the byte is %d\n",end - start);
    dst[end - start] = '\0';

    return dst;

}


int assemble_url(char *url,const char *request_line, const char *http_host)
{
    
    char url_ready[BUFFER]={0};
    char *start = NULL;
    int flag = 0;

    strcpy(url_ready, request_line);
    start = strstr(url_ready,"?");

    if(start != NULL)
            *start = '\0';

    strcpy(url,url_ready); 
        
    #if 0
    if(regex_compare(http_host,"^(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[0-9]{1,2})(\\.(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[0-9]{1,2})){3}$") >= 0)
    {
        strcpy(url,url_ready);
        flag = 1;// the hsot is ip
    }
    else
    {
        strcpy(url, http_host);
        strcat(url,"/");
        strcat(url,url_ready);
        flag = 0;//the host is server name
    }

    #endif

    return flag;
}

char *myrchr(char *src, char flag , int size)
{
   int i = 1;
   while(i < size)
   {
        if(src[size - i] == flag)
            return &src[size-i];
         i++;
   }

   return NULL;
}

int pre_file_name_check(char *file_name ,char *request_line ,int len )
{
    char *end = NULL;
    char *start = NULL;
    const char *p = request_line;
    int file_len = 0;

    end = strchr(request_line , '?');
    
    if(end != NULL)
    start = myrchr(request_line,'/', end - request_line);
    else
    start = strrchr(request_line,'/');



    if(end != NULL) /*  /abc/cd?abc */
    {
        if(start != NULL){
            start++;
            memcpy(file_name ,start, end - start);
            file_name[end-start] = '\0';
                
        }else{
            memcpy(file_name, p, end - p);
            file_name[end-p] = '\0';
        }

    }
    else{   /* /cd/dd  */
        if(start != NULL){
            start++;
            strcpy(file_name, start);
        }else{
            strcpy(file_name,p);
        }

    }

    if(strchr(file_name , '.') == NULL)
        return -1;
    else
        return 0;


}

int delete_http_version(char *request_line)
{
    int i = 1;
    int size = strlen(request_line);

    char *p = request_line;

    while(i < size)
    {
        p[size -i] = '\0'; 
        i++;
        if(p[size - i] == ' ')
        {
            p[size - i] = '\0';
            return 0;
        }
    }

    return -1;
      
}



 int compare_ip(const char *range ,u_int ip)
{
    char *p =NULL;
    char range_ip[100];
    int result = 0;
    int flag = 0;
    int i = 0;
    int c1=0;
 
    u_int dst_ip = 0;
    u_int src_ip = ip;
    int c = 0x0;

     strcpy(range_ip, range);



    p = strchr(range_ip,'/');
   if(p == NULL){
    //   printf("NOT FOUNT THE /\n");
        dst_ip = inet_addr(range_ip);
        if((dst_ip - src_ip) ==0)
            return result;
         else
            return -1;
     }
     *p = '\0';
     p++;
 
     dst_ip = inet_addr(range_ip);

 
     //printf("the dst_ip is %x, the src_ip is %x\n", dst_ip , src_ip);



     flag = atoi(p);
    //printf("the flag is %d\n",flag);

     
   for(i = 0; i < flag; i++ ){
        c1 = 0x1<<i;
         c = c + c1;
  }

   // printf("the result is %x\n",c&src_ip);

     if(((c&dst_ip) - (c&src_ip))==0)
       return 0;
    else
         return -1;


}//end of compare_ip

int str_compare(const char *http, const char *parse)
{

    int i = 0;
    char *p =NULL;
    char buf[100] ={0};
    const char *q = http;
    strcpy(buf, parse);


    p = strtok(buf,"*");

    if(p != NULL){

    while(p != NULL){
        if((q = strstr(q,p)) == NULL)
        {
		return -1;
        }else{
		q = q + strlen(p);
	}

	p = strtok(NULL,"*");
    
    
    }
    	return 0;
    }
    else
    {
        if(strcmp(http,parse) == 0){
            return 0;
        }else
          return  -1;
    
    
    }



}//0:match failed 1:match success


int str_compare_http_host(const char *http, const int handle_method)
{
    char *p = NULL;
    int  pmatch_eo = 0;
    int i = 0;
    int j = 0;

    


 if(handle_method == 0)
 {
    //while((next_char = mmap_gets(rule,100,next_char)) != NULL)
    for(; i < now_count_white; i++)
    {


        //printf("the file host is %s\n",rule);


            if(str_compare(http, white_rule[i].string) >= 0)
             {

                    for(j = 0; j < now_count_second_filter; j++){
                        if((str_compare(http,second_filter[j].string)) >= 0)
                         {
                             return 0;//match success
                         }
                    }
             }

     }
     return -1;
 }

 if(handle_method == 1)
 {
   for(; i < now_count_black; i++)
   {
       //printf("the file host is %s\n",rule);
           
           if(str_compare(http, black_rule[i].string) >= 0)
            {  
               return 0;//match success!
            
            }   
   }
 
    return -1;
 }


}


void get_sys_time(char *buf)
{
      struct timeval tt;
      struct tm tc;

      while(gettimeofday(&tt,NULL) < 0);

      while(localtime_r(&tt.tv_sec, &tc) == NULL);

      sprintf(buf,"%d%02d%02d.%02d%02d%02d.%d",tc.tm_year+1900, tc.tm_mon + 1, tc.tm_mday, tc.tm_hour, tc.tm_min, tc.tm_sec, tt.tv_usec);

      return ;
}

 void my_daemon(void)
 {
    pid_t pid;

    if((pid = fork()) < 0){
         perror("fork!");
         exit(1);
    }else if(pid != 0)
         exit(0);

          setsid();
     if(chdir("/") < 0){
          perror("chdir");
          exit(1);
     }
 
     close(0);
     open("/dev/null" , O_RDWR);
     dup2(0,1);
     dup2(0,2);
 }

void init_proc_title(int argc, char **argv) {
    int i;
    arg_start = argv[0];
    arg_end = argv[argc-1] + strlen(argv[argc-1])+1;
    env_start = environ[0];
    for(i=0; i<argc; i++)
        argv[i] = strdup(argv[i]);
}

void set_proc_title(const char *title) {
    int tlen = strlen(title)+1;
    int i;
    char *p;

    if(arg_end-arg_start < tlen && env_start==arg_end) {
        char *env_end = env_start;
        for(i=0; environ[i]; i++) {
            if(env_end == environ[i]) {
                env_end = environ[i] + strlen(environ[i]) + 1;
                environ[i] = strdup(environ[i]);
            } else
                break;
        }
        arg_end = env_end;
        env_start = NULL;
    }
    i = arg_end - arg_start;
    if(tlen==i) {
        strcpy(arg_start, title);
    } else if(tlen < i) {
        strcpy(arg_start, title);
        memset(arg_start+tlen, 0, i-tlen);
        // 1����Ҫ��ĵĽ����ƴ���ԭʼ�����ƴ���ʱ�����argv[0]�ֶ�ʱ����Ϊ���argv[0]��ĺ�Σ�ǰ�����0
        memset(arg_start,0,i);
        strcpy(arg_start + (i - tlen),title);
    } else {
        memmove(arg_start, title, i-1);
       // *(char *)mempcpy(arg_start, title, i-1) = '\0';
    }
    if(env_start) {
        p = strchr(arg_start, ' ');
        if(p) *p = '\0';
    }
}

void set_proc_name(const char *name) {
    prctl(PR_SET_NAME, name);
}

void get_proc_name(char *name) {
    prctl(PR_GET_NAME, name);
}


/*
*init_proc_title(argc,argv);
*set_proc_title(string);
*set_proc_name(string);
*/







