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
	int i = 0;
	int ret = 0;
	int sockfd[1024*10] = {0};
	for(i = 0; i < 10; i++) //建立多个长连接 是socket入门 //最多1024
	{
		sockfd[i] =  socket(PF_INET, SOCK_STREAM, 0);
		if (sockfd[i] == -1)
		{
			perror("fun socket\n");
			exit(0);
		}

		struct sockaddr_in  srvaddr;
		srvaddr.sin_family = AF_INET;
		srvaddr.sin_port = htons(8001);
		srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		if ( connect(sockfd[i], (struct sockaddr*) (&srvaddr), sizeof(srvaddr)) < 0)
		{
			perror("fun socket\n");
			exit(0);
		}
		else
		{
			printf("i=%d\n",i);
		}
	}
	char revbuf[1024] = {0};
	char sendbuf[1024] = {0};
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{
		write(sockfd[0], sendbuf, strlen(sendbuf)); 
		//从服务器读数据
		read(sockfd[0], revbuf, sizeof(revbuf));
		fputs(revbuf, stdout); //从服务器收到数据，打印屏幕
		memset(revbuf, 0, sizeof(revbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
	}
	for(i = 0; i < 10; i++)
	{
		close(sockfd[i]);
	}

	return 0;
}
