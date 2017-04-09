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

int main(void)
{
	int lfd = 0;
	lfd =  socket(PF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("fun socket\n");
		exit(0);
	}
	signal(SIGCHLD,SIG_IGN);

	struct sockaddr_in  srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8001);
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int optval = 1;
	if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}

	if ( bind( lfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
	{
		perror("fun bind\n");
		exit(0);
	}

	if ( listen(lfd, SOMAXCONN)<0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	struct sockaddr_in  peeraddr; //tcpip地址结构
	socklen_t peerlen = sizeof(peeraddr);
	char revbuf[1024] = {0};

	while(1)
	{
		unsigned int cfd ;
		//典型的阻塞式写法
		cfd = accept(lfd, (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen );
		if( cfd == -1)
		{
			close(lfd);
			perror("fun listen\n");
			exit(0);
		}

		int pid = fork();
		if( pid == 0)
		{
			close(lfd); //子进程不需要监听 //但是close lfd会不会影响 其他进程的accept? 难道accept时候 又会重新打开lfd 应该是的 //写到这里明显高别人一个档次 
			printf("peeradd:%s\n peerport:%d\n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));
			while(1)
			{
				int ret  = read( cfd, revbuf, sizeof(revbuf) );
				if(ret == 0)
				{
					printf("client close\n");
					break;
				}
				else if(ret < 0)
				{
					perror("read err");
					exit(0);	
				}

				fputs(revbuf, stdout);
				write(cfd, revbuf, ret);
			}
			close(cfd);
			exit(0);
		}
		else if( pid > 0)
		{
			printf("p*\n");
			close(cfd); //父进程不需要处理连接 
		}
		else 
		{
			perror("fork error\n");
			break;
		}
	}
	close(lfd);
	return 0;
}
//之所以有僵尸 进程 不是因为根本没有调回收子进程的函数 而是阻塞到accept上了
// 错 ？  父进程是个while大循环
// while 一直在干活无法回收 
// 要么忽略 做个不负责任的爹
// signal(SIGCHLD,SIG_IGN)
