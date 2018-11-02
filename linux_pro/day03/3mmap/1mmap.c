#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

void sys_err(char *str) {
  perror(str);
  exit(1);
}

int main(void) {
  char *mem;
  //    int fd = open("hello244", O_RDONLY|O_CREAT|O_TRUNC, 0644);
  int fd = open("dict.txt", O_RDWR);
  if (fd < 0) sys_err("open error");
  /*
	 len = lseek(fd, 3, SEEK_SET);   //获取文件大小,根据文件大小创建映射区
	 write(fd, "e", 1);              //实质性完成文件拓展
	 printf("The length of file = %d\n", len);
	 */
  //mem = (char*)mmap(NULL, 40960, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 8192);
  mem = (char*)mmap(NULL, 40960, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
  //para1一般传空 让内核对齐
  //para2不能为0  malloc可以为0
  if (mem == MAP_FAILED) sys_err("mmap err: ");
  close(fd);

  strcpy(mem, "xxx");
  printf("%s\n", mem);

  if (munmap(mem,  4) < 0) sys_err("munmap");
  return 0;
}
/*
可以open的时候O_CREAT一个新文件来创建映射区吗 不行必须设定文件长度不为0
如果mem++ munmap可否成功 失败
如果open时O_RDONLY mmap时PROT参数指定PROT_READ|PROT_WRITE会怎样  不行权限必须对等 权限必须有读
如果文件偏移量为1000会怎样 必须4k整数倍
对mem越界操作会怎样 竟然不会报错 又是4K
*/
