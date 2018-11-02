#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void) {
  printf("mpid = %d, main_parent_pid = %d\n", getpid(), getppid());
  pid_t pid;
  pid = fork(); //场景是两块重复的代码块
  if (pid == -1 ) {
	perror("fork");
	exit(1);
  } else if (pid > 0) { //this pid is child's pid 
	while (1) {
	  printf("parent_pid = %d, parent_parent_id = %d\n", getpid(), getppid());
	  sleep(1);
	}
  } else if (pid == 0) {
	while (1) {
	  printf("child_pid = %d, child_parent_id = %d\n", getpid(), getppid());
	  sleep(1);
	}
  }
  return 0;
}
