#include <stdio.h>
#include <unistd.h>
/*
l(list)    命令行参数列表
p(path)    搜素file时使用path变量
v(vector)  使用命令行参数数组
e(environment)       使用环境变量数组,不使用进程原有的环境变量，设置新加载程序运行的环境变量
*/

int main() {
  //execl("/bin/ls","ls","-l","-a",NULL); //手动添加可执行文件的搜寻路径  //注意NULL别忘写
  //execlp("ls","ls","-l","-a",NULL); //p即在path中找
  //char *argv[]={"ls","-l","-a",NULL};
  char *argv[]={"ok", NULL};
  execv("./wrong", argv); //传入参数
  perror("error:");
  printf("bye bye!\n"); //一去不复返 这一行不会打印
  return 0;
}

int main1(void) { //实用场景
  pid_t pid;
  pid = fork();
  if (pid == -1) {
	perror("fork error");
	exit(1);
  } else if (pid > 0) {
	sleep(1);
	printf("parent pid= %d, getppid = %d\n", getpid(), getppid());
  } else if (pid == 0) {
	char *argv[] = {"ls", "-l", "-a", "-F", NULL};
	//        execl("/bin/ls", "ls", "-l", "-a", "-F", NULL);
	//        execl("./test", "test",  NULL);
	execvp("ls", argv); //加载一个进程 并使用自定义的环境变量
	perror("execlp");
	exit(1);
  }
  return 0;
}
