#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/*自定义的信号捕捉函数*/
void sig_int(int signo) {
  printf("catch signal SIGINT\n");//单次打印
  printf("sleeping... 5 s\n"); //睡着5s 发送quit被屏蔽
  sleep(5);
  printf("sleep over\n");
}

int main(void) {
  struct sigaction act;		
  act.sa_handler = sig_int;
  act.sa_flags = 0;
  sigemptyset(&act.sa_mask);		//不屏蔽任何信号
  sigaddset(&act.sa_mask, SIGQUIT); //int被捕捉期间屏蔽quit信号
  sigaction(SIGINT, &act, NULL);
  printf("----main----\n");
  //sleep(5);
  while(1);//该循环只是为了保证有足够的时间来测试函数特性
  return 0;
}
