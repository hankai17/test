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
	printf("%d child,pid=%u,will die,pgid=%d\n",i+1,getpid(),getpgrp());
  } else {
	int n = 5;
	while(1) {
	  //while((pid=waitpid(-1,NULL,WNOHANG)) > 0 )
	  // {
	  //     printf("release child %u\n",pid);
	  //n--;
	  // }
	  pid = waitpid(0, NULL, WNOHANG);//WNOHANG不阻塞 三个返回值 0代表当前无子进程退出 >0代表有子进程退出 -1代表所有子进程意见回收完毕
	  if(pid > 0) printf("relase pid %d\n",pid);
	  if(pid < 0) break;
	  sleep(1);
	} 
    while(1);
  }
  return 0;
}

