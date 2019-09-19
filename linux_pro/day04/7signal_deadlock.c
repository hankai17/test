#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
int a=99;


void do_sig(int signum) {
  printf("in signal: %ld\n", pthread_self());
  pthread_mutex_lock(&mutex);
  printf("do_sig...\n");
  a = 0;
  pthread_mutex_unlock(&mutex);
  return;
}

void * thread_func_one(void *arg) {
  int i = 0;
  sigset_t signal_mask; //屏蔽47
  sigemptyset (&signal_mask);
  sigaddset(&signal_mask, 47);
  int rc = pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);
  if(rc != 0) {
      printf("block sigpipe error\n");
  }

  while(1) {
    pthread_mutex_lock(&mutex);
    for(i = 0; i < 10; i++) {
        printf("in thread:%ld %d...\n", pthread_self(), i + 1);
        //pthread_mutex_lock(&mutex);
        sleep(1);
    }
    pthread_mutex_unlock(&mutex);
    printf("-----------------\n");
    sleep(1); //必须睡
  }

  return NULL;
}

int main() { //起线程1一直占用锁  主线程接受信号然后调信号函数 信号回调中阻塞等待锁      ------>如果线程1不sleep1则信号回调中一直拿不到锁  然后傻屌测试再去发信号 而此时接受信号的不是主线程了而是线程1 又造成死锁
                                                                                                //造成死锁的原因是什么?  就是线程1接受了信号   应该让线程1屏蔽这个信号
  pthread_t pid;
  if (signal(47, do_sig) == SIG_ERR) {
    perror("signal");
    exit(1);
  }
  pthread_mutex_init(&mutex, NULL);
  if(0 != pthread_create( &pid, NULL, thread_func_one, NULL)) {
     printf("pthread create failed!/n");
     return -1;
  }
  pthread_join(pid, NULL);
  return 0;
}

int main1(void) { //只有一个主线程  主进程一直占用锁   一旦信号来了则打断主进程再去加锁  造成死锁
  if (signal(47, do_sig) == SIG_ERR) {
    perror("signal");
    exit(1);
  }
  pthread_mutex_init(&mutex, NULL);
  int i = 0;
  while(1) {
    pthread_mutex_lock(&mutex);                                                                                                                                                                                        
    for(i = 0; i < 10; i++) {                                                                                                                                                                                          
        printf("in main %d...\n", i + 1);                                                                                                                                                                              
        //pthread_mutex_lock(&mutex);                                                                                                                                                                                  
        sleep(1);                                                                                                                                                                                                      
    }                                                                                                                                                                                                                  
    pthread_mutex_unlock(&mutex);                                                                                                                                                                                      
    printf("-----------------\n");                                                                                                                                                                                     
  }                                                                                                                                                                                                                    
  return 0;                                                                                                                                                                                                            
}                                                                                                                                                                                                                      
                                                                                                                                                                                                                       
int main0(void) { //必须-lpthread  如果不链就不会出现死锁 很奇怪                                                                                                                                                       
  pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;                                                                                                                                                                  
                                                                                                                                                                                                                       
  pthread_mutex_lock(&mutex1);                                                                                                                                                                                         
  a = 777;                                                                                                                                                                                                             
  pthread_mutex_lock(&mutex1);                                                                                                                                                                                         
  pthread_mutex_unlock(&mutex1);                                                                                                                                                                                       
                                                                                                                                                                                                                       
  printf("-----------a = %d\n", a);                                                                                                                                                                                    
  pthread_mutex_destroy(&mutex1);                                                                                                                                                                                      
  return 0;                                                                                                                                                                                                            
}                                          
