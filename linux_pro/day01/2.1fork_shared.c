#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int var = 100;

int main() {
  pid_t pid = fork();
  if(pid == -1) {
	perror("fork err:");
	exit(1);
  } else if(pid == 0) {
	sleep(1);
	printf("child, pid=%u, ppid=%ui, var=%d\n",getpid(),getppid(),var);
	//printf("var_addr:%d\n",(int)(&var));
  } else {
	printf("parent, pid=%u, ppid=%u, var=%d\n",getpid(),getppid(),var);
	var = 51;
	sleep(1);
	printf("after, parent, pid=%u, ppid=%u, var=%d\n",getpid(),getppid(),var);
	//printf("var_addr:%d\n",(int)&var);
  }
  return 0;
}

