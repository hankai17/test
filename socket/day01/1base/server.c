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

int main(void)
{
	int skdstor = 0;
	skdstor =  socket(AF_INET, SOCK_STREAM , 0);

	struct sockaddr_in addrin;
	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(8001);//把一个短整型的本地字节序转成网络字节序   通过man可查到相关信息
	addrin.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*
	int optval = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}
	*/

	if(bind(skdstor, (const struct sockaddr *)&addrin,  sizeof(addrin)) == -1)
	{
		perror("fun bind\n");
		exit(0);
	}

	if ( listen(skdstor, SOMAXCONN) <0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	struct sockaddr_in  peeraddr;
	socklen_t peerlen = sizeof(peeraddr);

	unsigned int conn = 0;
	conn = accept(skdstor,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if (conn == -1)
	{
		perror("fun listen\n");
		exit(0);
	}

	char revbuf[1024] = {0};


	while(1)
	{

		int ret = read(conn, revbuf, sizeof(revbuf));
		if (ret == 0)
		{
			printf("对方已关闭  \n");
			exit(0);
		}
		else if (ret < 0)
		{
			perror("读数据失败\n");
			exit(0);
		}

		fputs(revbuf, stdout); //服务器端收到数据，打印屏幕
		write(conn, revbuf, ret); 
	}
	close(conn);
	close(skdstor);
	return 0;
}
