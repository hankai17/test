#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

void sys_err(char *str) {
  perror(str);
  exit(-1);
}

int main(int argc, char *argv[]) {
  int fd, i;
  int ret;
  char buf[4096];

  if (argc < 2) {
	//printf("Enter like this: ./a.out fifoname\n");
	//return -1;
	ret = mkfifo("myfifo", 0755);        
	if(ret!=0) {
	  perror("mkfifo err");
	  exit(1);
	}
  }
  fd = open("myfifo", O_WRONLY); //makefifo之后 还得打开 这跟pip不一样
  if (fd < 0) sys_err("open");
  i = 0;
  while (1) {
	sprintf(buf, "hello %d\n", i++);
	write(fd, buf, strlen(buf));
	sleep(1);
  }
  close(fd);
  return 0;
}
