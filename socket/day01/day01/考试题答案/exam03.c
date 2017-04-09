#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
/*
1因为孩子1的pid和孩子2的pid全部在父进程中
2 父进程把孩子2的pid，发给孩子1

3、孩子1有了孩子2的pid，孩子1就可以给孩子2发送信号了。
*/


int pidArray[10];   //写时拷贝copy-on-write 如果全区变量值改变，子进程另外拷贝

void myhandle(int num,siginfo_t *st, void *p)
{
	int val = 0;
	//printf("recve num:%d,data=%d\n",num,st->si_value.sival_int);
	if (num == SIGRTMIN+1)
	{
		//孩子1接收到了信号
		printf("孩子1收到信号，值:%d \n", st->si_value.sival_int);//父进程将子进程2的pid这个数据发给了，子进程1
		pidArray[1] = st->si_value.sival_int;// 子进程1将从父进程收到的数据放在本全局区pidArray[1]中
	}
	
	if (num == SIGRTMIN+2)
	{
		//孩子2接收到了信号
		printf("孩子2收到信号，值:%d \n", st->si_value.sival_int);//子进程1将自己的2*pid值发给子进程2
		pidArray[3] = st->si_value.sival_int;//子进程2将子进程1发送的数据存到本全局区中
	}
	
	if (num == SIGRTMIN+3)
	{
		//父进程接收到了信号
		printf("父进程收到信号，值:%d \n", st->si_value.sival_int);//子进程2将自己收到的数据又发给父进程
		printf("父进程中的pid[0]:%d pid[1]:%d \n", pidArray[0], pidArray[1]);//输出自己全局区中原来存的数据
		pidArray[3] = st->si_value.sival_int;//父进程将从子进程2收到的数据存入本全局区中
	}
}


int main()
{
	int i = 0;
	struct sigaction act;
	act.sa_sigaction=myhandle;
	act.sa_flags=SA_SIGINFO;
	  //注册信号 处理函数
	if(sigaction(SIGRTMIN+1,&act,NULL)==-1)
	{
	  	perror("func sigaction err:");
	  	return -2;
	 }
		  //注册信号 处理函数
	if(sigaction(SIGRTMIN+2,&act,NULL)==-1)
	{
	  	perror("func sigaction err:");
	  	return -2;
	 }
	 
	 	  //注册信号 处理函数
	if(sigaction(SIGRTMIN+3,&act,NULL)==-1)
	{
	  	perror("func sigaction err:");
	  	return -2;
	 }
	 
	 int pid = 0;
	 for (i=0; i<2; i++)
	 {
	 	//父进程可以把所有的孩子ID缓存下来
 		pidArray[i]  = pid = fork();
		if (pid == 0)
		{
			break; //如果是孩子，不参与fork
		}
		else if (pid > 0)
		{
			;
		}
 	}


	if (pid > 0)
	{
		printf("父进程运行\n");
		//父进程给进程1发送信号 把进程2的pid发过去
		  //添加额外数据
		  union sigval mysigval;
		  mysigval.sival_int=pidArray[1];

		  //发送消息
		  int res;
		  res = sigqueue(pidArray[0],SIGRTMIN+1, mysigval);

	}
	//子进程1
	if (pid==0 && i==0)
	{
		printf("子进程1运行\n");
		printf("子进程sleep\n");
		sleep(5);
		printf("子进程sleep被打断以后，全局变量有值了\n");
		
		  //添加额外数据
		  union sigval mysigval;
		  mysigval.sival_int=getpid()*2;

		  //发送消息
		  int res;
		  printf("子进程1给进程2发送信号getpid()*2:%d  \n", getpid()*2);
		  res = sigqueue(pidArray[1],SIGRTMIN+2, mysigval);
		
		exit(0);
	}
	
	
	//子进程2
	if (pid==0 && i==1)
	{
		
		sleep(10);
		printf("子进程2睡眠醒来，开始发信号\n");
		
		 //添加额外数据
		  union sigval mysigval;
		  mysigval.sival_int=pidArray[3];

		  //发送消息
		  int res;
		  printf("子进程2给父进程发送信号pidArray[3]%d  \n", pidArray[3]);
		  res = sigqueue(getppid(),SIGRTMIN+3, mysigval);
		
		exit(0);
	}
	
	sleep(2);
	int mypid = 0;
	while ( (mypid = waitpid(-1, NULL, WNOHANG)) > 0)
	{
		printf("子进程退出pid:%d \n", mypid);
	}
	
	return 0;

}

