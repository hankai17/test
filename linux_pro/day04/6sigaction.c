#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void do_sig(int signum) {
  printf("catch SIGINT----\n");
}

int main(void) {
  struct sigaction act;
  act.sa_handler = do_sig;
  act.sa_flags = 0; //不屏蔽任何信号
  sigemptyset(&act.sa_mask);
  sigaction(SIGINT, &act, NULL); //捕捉期间只屏蔽int信号  不支持排队

  while (1) {
	printf("---------------------\n");
	sleep(1);
  }
  return 0;
}
