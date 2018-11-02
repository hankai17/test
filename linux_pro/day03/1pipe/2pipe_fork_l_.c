#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
  pid_t pid;
  int fd[2];
  pipe(fd);
  pid = fork();

  if (pid == 0) {
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO); //让wc从管道中读取数据
	execlp("wc", "wc", "-l", NULL);	//wc命令默认从标准读入取数据
  } else {
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);	//将ls的结果写入管道中
	execlp("ls", "ls", NULL); //ls输出结果默认对应屏幕
  }
  return 0;
}

/*
 *  程序不时的会出现先打印$提示符，再出程序运行结果的现象。
 *  这是因为：父进程执行ls命令，将输出结果给通过管道传递给
 *  子进程去执行wc命令，这时父进程若先于子进程打印wc运行结果
 *  之前被shell使用wait函数成功回收，shell就会先于子进程打印
 *  wc运行结果之前打印$提示符。
 *  解决方法：让子进程执行ls,父进程执行wc命令。或者在兄弟进程间完成。
 */

