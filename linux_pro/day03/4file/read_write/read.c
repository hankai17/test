#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
//不同进程间读写文件的一个坑 就是读写指针只有一个

int main(void) {
  char buf[1024];
  char *str = "----------test2 write secesuss--------\n";
  int ret;
  sleep(2);   //睡眠2秒,保证test1将数据写入test.txt文件

  int fd = open("test.txt", O_RDWR);
  ret = read(fd, buf, sizeof(buf));   
  write(STDOUT_FILENO, buf, ret);
  //fseek();

  //写入数据到文件test.txt中, 未修改读写位置
  write(fd, str, strlen(str));

  printf("test2 read/write finish\n");

  close(fd);

  return 0;
}
