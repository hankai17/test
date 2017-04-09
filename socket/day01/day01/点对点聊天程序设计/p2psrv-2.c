
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

void handle(int num)
{
	printf("recv num:%d \n",num);
	exit(0);
}


int main()
{
	int sockfd = 0;
	
    
 	signal(SIGUSR1,  handle);


	sockfd =  socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("fun socket\n");
		exit(0);
	}
	
	struct sockaddr_in  srvaddr;
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(8001);
	srvaddr.sin_addr.s_addr = inet_addr("192.168.6.249"); //127.0.0.1
	//srvaddr.sin_addr.s_addr = inet_addr(INADDR_ANY); //绑定本机的任意一个地址 
	
	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}
	
	if ( bind( sockfd, (struct sockaddr *)&srvaddr, sizeof(srvaddr)) < 0)
	{
		perror("fun bind\n");
		exit(0);
	}
	
	//一但调用listen函数，这个套接字sockfd将变成被动套接字;只能接受连接，不能主动的发送连接
	//listen 做了两个队列。。。。。。
	if ( listen(sockfd, SOMAXCONN)<0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	
	//struct sockaddr peeraddr;  //通用ip
	//socklen_t perrlen ;
	
	struct sockaddr_in  peeraddr; //tcpip地址结构
	socklen_t peerlen = sizeof(peeraddr);
	
	unsigned int conn = 0;
	//accept 返回一个新的连接 ，这个新的连接是一个主动套接字
	conn = accept(sockfd,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if (conn == -1)
	{
		perror("fun listen\n");
		exit(0);
	}
	printf("perradd:%s\n perrport:%d\n", inet_ntoa(peeraddr.sin_addr),  ntohs(peeraddr.sin_port));
	
	int pid;
	pid = fork();
	if (pid > 0)
	{
			char revbuf[1024] = {0};
			while (1)
			{
				int ret = read(conn, revbuf, sizeof(revbuf));
				if (ret == 0)
				{
					//如果在读的过程中，对方已经关闭，tcpip协议会返回一个0数据包
					printf("对方已关闭\n");
					
					break;
				}
				else if (ret < 0)
				{
					perror("读数据失败\n"); 
					exit(0);
				}
				//ssize_t write(int fd, const void *buf, size_t count);
				fputs(revbuf, stdout); //服务器端收到数据，打印屏幕
				
				memset(revbuf, 0, sizeof(revbuf));
			}
			close(conn);
			//kill(pid , SIGUSR1); 
			sleep(1);
	
	}
	else
	{
		//char revbuf[1024] = {0};
		char sendbuf[1024] = {0};
		while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
		{
			//向客户端写数据
			write(conn, sendbuf, strlen(sendbuf)); 
			memset(sendbuf, 0, sizeof(sendbuf));
		}
	}
	   

	close(sockfd);
	
	return 0; 	
}






 