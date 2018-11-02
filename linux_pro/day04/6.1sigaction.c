#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sig_int(int signo) {
  printf("catch signal SIGINT\n");
  sleep(10); //睡这10s 再次cc自动屏蔽本信号  直到10s结束后再次sleep 不支持排队   //但是发送quit会立即退出
  printf("end of handler\n");
}

int main(void) {
  struct sigaction act, old;		
  act.sa_handler = sig_int;
  sigemptyset(&act.sa_mask);		//依然不屏蔽任何信号
  act.sa_flags = 0;
  sigaction(SIGINT, &act, &old);	//注册信号处理函数
  while(1);
  sigaction(SIGINT, &old, NULL);	//注册信号处理函数
  return 0;
}
