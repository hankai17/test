#include <stdio.h>
#include <stdlib.h>

int main() {
  pid_t pid;
  int i = 0;
  for(i = 0 ; i < 5; i ++) {
	pid=fork();
	if(pid == -1) {
	  perror("fork err:");
	  exit(1);
	} else if(pid == 0) {
	  break;
	}
  }
  if(i < 5) {
	//sleep(i);
	printf("%d child,pid=%u,will die\n",i+1,getpid());
  } else {
	while((pid = wait(NULL)) > 0 ) { //wait阻塞等待父进程所属的所有子进程
	  printf("release child %u\n",pid);
	}
  }
  return 0;
}

