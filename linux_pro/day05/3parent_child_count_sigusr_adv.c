#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

//int n = 0, flag = 0;
int n = 0;
pid_t pid_c;


void sys_err(char *str)
{
	perror(str);
	exit(1);
}
void do_sig_child(int num)
{
	printf("I am child  %d\t%d\n", getpid(), n);
	n += 2;
	kill(getppid(), SIGUSR2);
	//sleep(1);
}
void do_sig_parent(int num)
{
	printf("I am parent %d\t%d\n", getpid(), n);
	n += 2;
	kill(pid_c, SIGUSR1);
	//sleep(1);
}
int main(void)
{
	pid_t pid;
	struct sigaction act;

	if ((pid = fork()) < 0)
		sys_err("fork");

	else if (pid > 0) 
	{     
pid_c=pid;
		n = 1;
		sleep(1);                                   //给子进程留足事件注册信号捕捉函数

		act.sa_handler = do_sig_parent;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR2, &act, NULL);             //注册自己的信号捕捉函数   父使用SIGUSR2信号

		do_sig_parent(0);

		while(1); 

	} 
	else if (pid == 0)
	{       
		n = 2;
		act.sa_handler = do_sig_child;
		sigemptyset(&act.sa_mask);
		act.sa_flags = 0;
		sigaction(SIGUSR1, &act, NULL);             //父进程数数完成发送SIGUSER1给子进程。

		while(1); 
	}

	return 0;
}
