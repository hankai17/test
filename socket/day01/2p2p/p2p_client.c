#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

void myhandle(int sig)
{
	printf("客户端父进程退出,parent recv sig=%d\n",sig);
	exit(0);
}

int main(void)
{
	int sock;
	sock =  socket(AF_INET, SOCK_STREAM , 0);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8001);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if( connect(sock,  (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect:");
		exit(0);
	}
	pid_t pid;
	pid = fork(); //父进程负责发 子进程收
	if(pid == -1)
	{
		perror("fork error:");
		close(sock);
		exit(0);
	}
	if(pid == 0) //子
	{
		char recvbuf[1024] = {0};
		while(1) 
		{
			memset(recvbuf, 0, sizeof(recvbuf));
			int ret = read(sock,recvbuf,sizeof(recvbuf));
			if(ret == -1)
			{
				perror("read err\n");
				break;
			}
			else if(ret == 0)
			{
				printf("peer close\n");
				break;
			}
			fputs(recvbuf,stdout);
		}
		close(sock);
		kill(getppid(),SIGUSR1);  //解决方法 告诉父进程死 //避免僵尸进程
		printf("客户端子进程退出\n");
		exit(0);
	}
	else
	{
		char sendbuf[1024] = {0};
		signal(SIGUSR1,myhandle);
		printf("客户端父进程要求输入数据：\n");
		while( fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)  //子进程死了，父进程阻塞于此
		{
			printf("客户端获取数据长度:%d\n",strlen(sendbuf));
			write(sock,sendbuf,strlen(sendbuf));
			memset(sendbuf,0,sizeof(sendbuf));
		}
		printf("客户端父进程退出\n");
	}
	return 0;
}
//建议 画那个图 更清晰一些
