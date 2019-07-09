#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void printset(sigset_t *ped) {
  int i;
  for(i = 1; i < 32; i++) {
	if((sigismember(ped, i) == 1)) {
	  putchar('1');
	} else {
	  putchar('0');
	}
  }
  printf("\n");
}

int main(void) {
  sigset_t set, oldset, ped;
  sigemptyset(&set);
  sigaddset(&set, SIGINT); //cc
  sigaddset(&set, SIGQUIT); //c、、
  sigaddset(&set, SIGKILL); //kill -9
  sigaddset(&set, SIGSTOP); //cz 即使设置了kill stop也是执行默认动作 而非捕捉
  sigaddset(&set, SIGSEGV);

  //sigprocmask(SIG_BLOCK, &set, &oldset);	
  sigprocmask(SIG_BLOCK, &set, &oldset); //设置屏蔽/阻塞

  while(1) {
	sigpending(&ped); //读取未决信号集
	printset(&ped);
	sleep(1);
  }
  return 0;
}

//block --> pending --> deliver 必须有这个场景
