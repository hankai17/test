#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
  pid_t pid = fork();
  if(pid == -1) {
	perror("fork err:");
	exit(1);
  } else if(pid == 0) {
	while(1) {
	  printf("child pid=%u,ppid=%u\n",getpid(),getppid());
	  sleep(1);
	}
  } else {
	while(1) {
	  sleep(1);
	  printf("parent!\n");
	}
  }
  return 0;
}
