#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "ares.h"

ares_channel channel;
pthread_mutex_t mutex;
int thread_num = 1;

static void callback1(void* arg, int status, struct hostent* host) {
  struct in_addr addr;
  char** p;
  if (status != ARES_SUCCESS) {
    fprintf(stderr, "%s: %s\n", (char *) arg, ares_strerror(status));
    return;
  }

  for (p = host->h_addr_list; *p; p++) {
      memcpy(&addr, *p, sizeof(struct in_addr));
      printf("%-32s\t%s\n", host->h_name, inet_ntoa(addr));
  }
}

void* tfn(void* arg) {
    while(1) {
      //pthread_mutex_lock(&mutex);
      ares_gethostbyname(channel, "www.jd.com", AF_INET, callback1, "www.jd.com");
      ares_gethostbyname(channel, "www.ifeng.com", AF_INET, callback1, "www.ifeng.com");
      ares_gethostbyname(channel, "www.taobao.com", AF_INET, callback1, "www.taobao.com");
      pthread_mutex_unlock(&mutex);
      sleep(rand() % 2);
    }
}

int main(int argc, char **argv) {
  int status, nfds;
  fd_set read_fds, write_fds;
  struct timeval *tvp, tv;
  struct in_addr addr;
  int tid, i;

  status = ares_init(&channel);
  if(status != ARES_SUCCESS) {
      fprintf(stderr, "ares_init: %s\n", ares_strerror(status));
      return 1;
  }

  pthread_mutex_init(&mutex, NULL);
  for(i = 0; i < thread_num; i++) {
    pthread_create(&tid, NULL, tfn, NULL);
  }
  /*
  ares_gethostbyname(channel, "www.baidu.com", AF_INET, callback1, *argv);
  ares_gethostbyname(channel, "www.jd.com", AF_INET, callback1, *argv);
  ares_gethostbyname(channel, "www.taobao.com", AF_INET, callback1, *argv);
  ares_gethostbyname(channel, "www.sina.com", AF_INET, callback1, *argv);
  ares_gethostbyname(channel, "www.youku.com", AF_INET, callback1, *argv);
  ares_gethostbyname(channel, "www.tudou.com", AF_INET, callback1, *argv);
  ares_gethostbyname(channel, "www.ifeng.com", AF_INET, callback1, *argv);
  ares_gethostbyname(channel, "www.baidu.com", AF_INET, callback1, *argv);
  //ares_gethostbyaddr(channel, &addr, sizeof(addr), AF_INET, callback1, *argv);
*/
    ares_gethostbyname(channel, "www.baidu.com", AF_INET, callback1, *argv);
  while (1) {
      FD_ZERO(&read_fds);
      FD_ZERO(&write_fds);
      nfds = ares_fds(channel, &read_fds, &write_fds);
      //if (nfds == 0) break;
      if (nfds == 0) continue;
      tvp = ares_timeout(channel, NULL, &tv);
      select(nfds, &read_fds, &write_fds, NULL, tvp);
      ares_process(channel, &read_fds, &write_fds);
  }

  ares_destroy(channel);
  return 0;
}
