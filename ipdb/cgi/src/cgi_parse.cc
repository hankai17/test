#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "fcgi_stdio.h"
#include "c_search.hh"
#include "util_cgi.h"

int main(int argc, char *argv[])
{
  //int count = 0;
  char* content_buf = NULL;
  std::ifstream f;
  char* ipdb_buf;
  int file_size;
  if(init_file(f, &ipdb_buf, &file_size)) {
    std::cout<<"init_file err\n";
    return 0;
  }

  char* val = (char*)malloc(1024);
  memset(val, '\0', 1024);
  int val_len = 0;

  while (FCGI_Accept() >= 0) {
    printf("Content-type: text/html\r\n");
    printf("\r\n");
    //printf("<title>Fast CGI Hello!</title>");
    //printf("<h1>Fast CGI Search Ip!</h1>");
    //printf("Request number %d running on host <i>%s</i>\n",
    //    ++count, getenv("SERVER_NAME"));

    char* content_len = getenv("CONTENT_LENGTH");
    int len = 0;
    int i = 0;
    int ch = 0;
    char* p = NULL;

    if(content_len != NULL) {
      len = strtol(content_len, NULL, 10);
      if (len < 0) len = 0;
    } else {
      len = 0;
    }
    content_buf = (char*) malloc(sizeof(char) * (len + 1));
    memset(content_buf,'\0',len + 1);
    if(content_buf == NULL) {
      printf("malloc err\n");
      return 0;
    }

    p = content_buf;
    for(i = 0; i < len; i++) {
      if((ch = getchar()) < 0) {
        printf("getchar err\n");
        break;
      }
      *p = ch;
      p++;
    }
    //printf("content_buf=%s\n",content_buf); //ip=1.1.1.1
    query_parse_key_value((const char*)content_buf, "ip", val, &val_len);
    std::string ip(val);
    std::string result;
    int ret = search_ip(ipdb_buf, file_size, ip, result);
    if(ret == 1) {
      printf("Not found! may be the IP you queried for belongs foreign countries!\n");
      ret = 0;
    } else if(ret == 2) {
      printf("The IP you input is err!\n");
      ret = 0;
    } else {
      printf("result %s\n",result.c_str());
    }

    if(content_buf != NULL) {
      free(content_buf);
      content_buf = NULL;
    }
  }
}
