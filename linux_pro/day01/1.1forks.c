#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  int n = 5, i;	//默认创建5个子进程
  if(argc == 2) {	
	n = atoi(argv[1]);
  }

  for(i = 0; i < n; i++) {
	printf("*\n");
	if(fork() == 0) //子
	break;
  }
  //场景是 5个重复的代码块
  if(n == i) {
	sleep(n);
	printf("I am parent, pid = %d\n", getpid());
  } else {
	sleep(i);
	printf("I'm %dth child, pid = %d\n", i+1, getpid());
  }
  return 0;
}
