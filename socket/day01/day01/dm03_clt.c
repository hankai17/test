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

/*
 int socket(int domain, int type, int protocol);
 */

/*
 int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);

 //man 2 bind
 struct sockaddr {
 sa_family_t sa_family;
 char        sa_data[14];
 }

 //man 7 ip
 struct sockaddr_in {
 sa_family_t    sin_family; // address family: AF_INET
 u_int16_t      sin_port;   // port in network byte order
 struct in_addr sin_addr;   // internet address
 };

 //Internet address.
 struct in_addr {
 u_int32_t      s_addr;     //address in network byte order
 };

 */

//int listen(int sockfd, int backlog);
//accept 接受一个新的连接 ，这个新的连接是一个主动套接字
/*   int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
 On success, accept() returns a non-negative integer that is a descriptor for the accepted socket.   On  error,
 -1 is returned, and errno is set appropriately.
 int conn = 0;
 */

int main()
{
	int sockfd = 0;
	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
	{
		perror("fun socket\n");
		exit(0);
	}
	//设置客服端链接的tcpip结构体信息
	struct sockaddr_in srvaddr;
	//协议族
	srvaddr.sin_family = AF_INET;
	//设置端口
	srvaddr.sin_port = htons(8001);
	//设置ip
	srvaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //127.0.0.1
	//srvaddr.sin_addr.s_addr = inet_addr(INADDR_ANY); //绑定本机的任意一个地址
	//客服端链接
	if (connect(sockfd, (struct sockaddr*) (&srvaddr), sizeof(srvaddr)) < 0)
	{
		perror("fun socket\n");
		exit(0);
	}
	//设置接受和发送的缓存。
	char revbuf[1024] = { 0 };
	char sendbuf[1024] = { 0 };
	//
	//char *fgets(char *s, int size, FILE *stream); 从stream 读取size-1大小的数据存入s,最后加'\0'
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{
		//向服务写数据
		//ssize_t write(int fd, const void *buf, size_t count);
		// 从buf中读取count大小的数据存入文件描述符为fd的文件中。
		write(sockfd, sendbuf, strlen(sendbuf)); //服务器端回发信息
		//从服务器读数据
		//ssize_t read(int fd, void *buf, size_t count);
		//从文件描述符为fd的文件中读取大小为count的数据存入buf中。
		read(sockfd, revbuf, sizeof(revbuf));
		fputs(revbuf, stdout); //从服务器收到数据，打印屏幕
		memset(revbuf, 0, sizeof(revbuf));
		memset(sendbuf, 0, sizeof(sendbuf));
	}
	close(sockfd);
	return 0;
}

