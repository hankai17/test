#include "prot.h"
#include "cmn_log.h"


void cancel_char(char *string)
{
    int i = strlen(string);
    i = i -1;
        while((string[i] == '\n') ||(string[i] == ' ')||(string[i] == '\r') )
        {
            string[i] = '\0';
            i--;
            if(i < 0)
                break;
        }
}


int get_message( FILE *fp ,char *match)
{
    char *p = NULL;
    char *q =NULL;
    char string[1024];
    char  *c = NULL;

   while((c = fgets(string, 1024, fp)) != NULL)
    {
        if(string[0] == '#')
            continue;
        else
            break;

    }

    if(c == NULL)
        return -1;

    p = strstr(string, "=");
    if(p == NULL)
    {
        return -1;
    }

    p++;
    strcpy(match ,p);
    cancel_char(match);
    return 0;
}


int get_conf_value( char *file,CF *conf)
{
    FILE *fp = NULL;
    fp = fopen(file, "r");
    if(fp == NULL)
    perror("open file failed!:");

          if( get_message(fp,conf->port) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the port!\n");
             fclose(fp);
            return -1;
          }

          if(get_message(fp,conf->dev) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the dev!\n");
            fclose(fp);
            return -1;
          }

          if(get_message(fp,conf->path_white) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the path_white\n");
            fclose(fp);
            return -1;
          }

          if(get_message(fp,conf->path_black) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the path_white\n");
            fclose(fp);
            return -1;
          }

          if(get_message(fp,conf->path_server_ip) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the server_ip\n");
            fclose(fp);
            return -1;
          }
          if(get_message(fp,conf->path_second_filter) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the second filter!\n");
            fclose(fp);
            return -1;
          }
          if(get_message(fp,conf->path_src_ip_white) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the path_src_ip_white !\n");
            fclose(fp);
            return -1;
          }
          if(get_message(fp,conf->path_src_ip_black) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the path_src_ip_black!\n");
            fclose(fp);
            return -1;
          }

          if(get_message(fp, conf->function) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the function choose!\n");
            fclose(fp);
            return -1;
          
          }
          if(get_message(fp,conf->path_http_white_list) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the path_http_white_list!\n");
            fclose(fp);
            return -1;
          }
          if(get_message(fp,conf->path_http_black_list) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the path_http_black_list!\n");
            fclose(fp);
            return -1;
          }
          if(get_message(fp,conf->path_keywords_filter_list) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the path_http_black_list!\n");
            fclose(fp);
            return -1;
          }
          if(get_message(fp,conf->dns_thread_num) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the dns_thread num!\n");
            fclose(fp);
            return -1;
          }
          if(get_message(fp,conf->http_thread_num) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found the http_thread_num!\n");
            fclose(fp);
            return -1;
          }
          if(get_message(fp,conf->insert_name) == -1)
          {
            CMN_LOG(MY_LOG_DEBUG, "no found insert name!\n");
            fclose(fp);
            return -1;
          }




    fclose(fp);
    return 0;

}


 
 int read_filter_list(char *filename ,const int method ) 
{ 
       char buffer[LEN] = {0}; 
       int size = 0; 
       FILE *fp = NULL; 
       char *cp = NULL; 


 
     fp = fopen(filename,"r"); 
      if(fp == NULL) 
     { 
        CMN_LOG(MY_LOG_DEBUG, "open the %s failed!",filename);
         return -1; 
     } 

    while(fgets(buffer, LEN, fp) != NULL){ 
      
     cp =strchr(buffer, '#'); 
     if(cp != NULL) 
            *cp = '\0'; 

     size = strlen(buffer); 
 
     while((size > 0)&& 
    (buffer[size - 1] == ' ' || buffer[size - 1] == '\t' || buffer[size-1] == '\n' || buffer[size -1] == '\r')){ 
     buffer[--size] = '\0';
  }

    if(size == 0)
         continue;

if(method == SECOND_FILTER)
{

  if(now_count_second_filter >= max_count_second_filter){
       if(now_count_second_filter == 0){
             max_count_second_filter = 100;
             second_filter = NEW(rule,100);
   }
   else{

         max_count_second_filter += 100;
           second_filter  = RENEW(second_filter,rule,max_count_second_filter);
       }

         if(second_filter  == NULL)
         {
             CMN_LOG(MY_LOG_DEBUG, "malloc the rule failed !\n");
             return -1;
         }

   }

    strcpy(second_filter[now_count_second_filter].string , buffer);
     
    now_count_second_filter++;
 }//end of second filter

if(method == BLACK_FILTER)
{
  if(now_count_black >= max_count_black){
       if(now_count_black == 0){
             max_count_black = 100;
             black_rule = NEW(rule,100);
   }
   else{

         max_count_black += 100;
          black_rule = RENEW(black_rule,rule,max_count_black);
       }

         if(black_rule == NULL)
         {
             CMN_LOG(MY_LOG_DEBUG, "malloc the rule failed !\n");
             return -1;
         }

   }

    strcpy(black_rule[now_count_black].string , buffer);
     
    now_count_black++;


}//end of black_filter

if(method == WHITE_FILTER)
{
  if(now_count_white >= max_count_white){
       if(now_count_white == 0){
             max_count_white = 100;
             white_rule = NEW(rule,100);
   }
   else{

         max_count_white += 100;
          white_rule = RENEW(white_rule,rule,max_count_white);
       }

         if(white_rule == NULL)
         {
             CMN_LOG(MY_LOG_DEBUG, "malloc the rule failed !\n");
             return -1;
         }

   }

    strcpy(white_rule[now_count_white].string , buffer);
     
    now_count_white++;




}//endoif white filter




  }//end of for()

    return 0;
 }






 int read_src_ip(char *filename ,const int method )//method ==0,src_ip_black, method =1 ,src_ip_white 
{ 
       char buffer[LEN] = {0}; 
       int size = 0; 
       FILE *fp = NULL; 
       char *cp = NULL; 


 
     fp = fopen(filename,"r"); 
      if(fp == NULL) 
     { 
        printf("open the %s failed!",filename); 
         return -1; 
     } 

    while(fgets(buffer, LEN, fp) != NULL){ 
      
     cp =strchr(buffer, '#'); 
     if(cp != NULL) 
            *cp = '\0'; 

     size = strlen(buffer); 
 
     while((size > 0)&& 
    (buffer[size - 1] == ' ' || buffer[size - 1] == '\t' || buffer[size-1] == '\n' || buffer[size -1] == '\r')){ 
     buffer[--size] = '\0';
  }

    if(size == 0)
         continue;

if(method == SRC_IP_BLACK)
{

  if(now_count_ip_black >= max_count_ip_black){
       if(now_count_ip_black  == 0){
             max_count_ip_black = 100;
             src_ip_black = NEW(rule,100);
   }
   else{

         max_count_ip_black += 100;
           src_ip_black = RENEW(src_ip_black,rule,max_count_ip_black);
       }

         if(src_ip_black   == NULL)
         {
             printf("malloc the src ip  black failed !\n");
             return -1;
         }

   }

    strcpy(src_ip_black[now_count_ip_black].string , buffer);
     
    now_count_ip_black++;
 }//end of src ip black filter

if(method == SRC_IP_WHITE)
{

  if(now_count_ip_white >= max_count_ip_white){
       if(now_count_ip_white  == 0){
             max_count_ip_white = 100;
             src_ip_white = NEW(rule,100);
   }
   else{

         max_count_ip_white += 100;
           src_ip_white = RENEW(src_ip_white,rule,max_count_ip_white);
       }

         if(src_ip_white   == NULL)
         {
             printf("malloc the src ip white  failed !\n");
             return -1;
         }

   }

    strcpy(src_ip_white[now_count_ip_white].string , buffer);
     
    now_count_ip_white++;

 }
 }//end of while(....)

 return 0;

 }


int read_http_filter_list(char *filename, const int method)//method = 0,http_black_filter method = 1,http_white_filter
{

       char buffer[LEN] = {0}; 
       int size = 0; 
       FILE *fp = NULL; 
       char *cp = NULL; 
 
     fp = fopen(filename,"r"); 
      if(fp == NULL) 
     { 
        printf("open the %s failed!",filename); 
         return -1; 
     } 

    while(fgets(buffer, LEN, fp) != NULL){ 
      
     cp =strchr(buffer, '#'); 
     if(cp != NULL) 
            *cp = '\0'; 

     size = strlen(buffer); 
 
     while((size > 0)&& 
    (buffer[size - 1] == ' ' || buffer[size - 1] == '\t' || buffer[size-1] == '\n' || buffer[size -1] == '\r')){ 
     buffer[--size] = '\0';
  }

    if(size == 0)
         continue;

if(method == HTTP_BLACK_FILTER)
{

  if(now_count_http_black_list >= max_count_http_black_list){
       if(now_count_http_black_list  == 0){
             max_count_http_black_list = 100;
             http_black_list = NEW(rule,100);
   }
   else{

         max_count_http_black_list += 100;
            http_black_list = RENEW(http_black_list,rule,max_count_http_black_list);
       }

         if(http_black_list   == NULL)
         {
             printf("malloc the http black list failed !\n");
             return -1;
         }

   }

    strcpy(http_black_list[now_count_http_black_list].string , buffer);
     
    now_count_http_black_list++;
 }//end of http black list  filter

if(method == HTTP_WHITE_FILTER)
{

  if(now_count_http_white_list >= max_count_http_white_list){
       if(now_count_http_white_list  == 0){
             max_count_http_white_list = 100;
             http_white_list = NEW(rule,100);
   }
   else{

         max_count_http_white_list += 100;
           http_white_list = RENEW(http_white_list,rule,max_count_http_white_list);
       }

         if(http_white_list   == NULL)
         {
             printf("malloc http white  ip  failed !\n");
             return -1;
         }

   }

    strcpy(http_white_list[now_count_http_white_list].string , buffer);
     
    now_count_http_white_list++;

 }
 }//end of while(....)

 return 0;

}



int read_server_ip(char *filename)
{

       char buffer[LEN] = {0}; 
       int size = 0; 
       FILE *fp = NULL; 
       char *cp = NULL; 
 
     fp = fopen(filename,"r"); 
      if(fp == NULL) 
     { 
        printf("open the %s failed!",filename); 
         return -1; 
     } 

    while(fgets(buffer, LEN, fp) != NULL){ 
      
     cp =strchr(buffer, '#'); 
     if(cp != NULL) 
            *cp = '\0'; 

     size = strlen(buffer); 
 
     while((size > 0)&& 
    (buffer[size - 1] == ' ' || buffer[size - 1] == '\t' || buffer[size-1] == '\n' || buffer[size -1] == '\r')){ 
     buffer[--size] = '\0';
  }

    if(size == 0)
         continue;


  if(now_count_server_ip >= max_count_server_ip){
       if(now_count_server_ip  == 0){
             max_count_server_ip = 100;
             server_ip = NEW(rule,100);
   }
   else{

         max_count_server_ip += 100;
             server_ip = RENEW(server_ip,rule,max_count_server_ip);
       }

         if(server_ip   == NULL)
         {
             printf("malloc the server_ip list failed !\n");
             return -1;
         }

   }

    strcpy(server_ip[now_count_server_ip].string , buffer);
     
    now_count_server_ip++;
 }//end of while()

 return 0;
 }


int read_keywords_filter_list(char *filename)
{

       char buffer[LEN] = {0}; 
       int size = 0; 
       FILE *fp = NULL; 
       char *cp = NULL; 
 
     fp = fopen(filename,"r"); 
      if(fp == NULL) 
     { 
        printf("open the %s failed!",filename); 
         return -1; 
     } 

    while(fgets(buffer, LEN, fp) != NULL){ 
      
     cp =strchr(buffer, '#'); 
     if(cp != NULL) 
            *cp = '\0'; 

     size = strlen(buffer); 
 
     while((size > 0)&& 
    (buffer[size - 1] == ' ' || buffer[size - 1] == '\t' || buffer[size-1] == '\n' || buffer[size -1] == '\r')){ 
     buffer[--size] = '\0';
  }

    if(size == 0)
         continue;


  if(now_count_keywords_filter >= max_count_keywords_filter){
       if(now_count_keywords_filter  == 0){
             max_count_keywords_filter = 100;
             keywords_filter = NEW(rule,100);
   }
   else{

         max_count_keywords_filter += 100;
             keywords_filter = RENEW(keywords_filter,rule,max_count_keywords_filter);
       }

         if(keywords_filter   == NULL)
         {
             printf("malloc the server_ip list failed !\n");
             return -1;
         }

   }

    strcpy(keywords_filter[now_count_keywords_filter].string , buffer);
     
    now_count_keywords_filter++;
 }//end of while()

 return 0;
 }











#if 0
int main(int argc, char *argv[])
{
    CF argument1;
    if( get_conf_value(argv[1],&argument1) == 0)
    {
        printf("get message success!");
        printf("the port is %s,the dev is %s,the path_white is %s,the path_black is %s\n", argument1.port, argument1.dev , argument1.path_white,argument1.path_black);
    }
    else
        printf("error !");



}
#endif

