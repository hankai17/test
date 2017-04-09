#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
        do \
        { \
                perror(m); \
                exit(EXIT_FAILURE); \
        } while(0)

ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nread;
	char *bufp = (char*) buf;

	while (nleft > 0)
	{
		if ((nread = read(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nread == 0)
			return count - nleft;

		bufp += nread;
		nleft -= nread;
	}

	return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count;//需要写入的数据个数
	ssize_t nwritten;//
	char *bufp = (char*) buf;

	while (nleft > 0)
	{
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nwritten == 0)
			continue;

		bufp += nwritten;
		nleft -= nwritten;
	}

	return count;
}
//从指定的socket中读取指定大小的数据但 不取出，封装后不被信号中断
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while (1)
	{
		//MSG_PEEK 读取队列中指定大小的数据，但不取出
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		//如果被信号中断，则继续
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

//maxline 一行最大数
//先提前peek一下缓冲区，如果有数据从缓冲区的读数据，
//1、缓冲区数据中带\n
//2 缓存区中不带\n
//读取读取包直到\n
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread;//成功预读取的数据的个数
	char *bufp = buf;//读取数据存放的数组，在外分配内存
	int nleft = maxline;//封包最大值
	while (1)
	{
		//看一下缓冲区有没有数据，并不移除内核缓冲区数据
		ret = recv_peek(sockfd, bufp, nleft);
		if (ret < 0) //失败 
			return ret;
		else if (ret == 0) //对方已关闭
			return ret;

		nread = ret;
		int i;
		for (i = 0; i < nread; i++)
		{
			if (bufp[i] == '\n') //若缓冲区有\n
			{
				ret = readn(sockfd, bufp, i + 1); //读走数据
				if (ret != i + 1)
					exit(EXIT_FAILURE);
				return ret; //有\n就返回，并返回读走的数据
			}
		}

		if (nread > nleft) //如果读到的数大于 一行最大数 异常处理
			exit(EXIT_FAILURE);

		nleft -= nread;  ////若缓冲区没有\n, 把剩余的数据读走
		ret = readn(sockfd, bufp, nread);
		if (ret != nread)
			exit(EXIT_FAILURE);
		bufp += nread; //bufp指针后移后，再接着偷看缓冲区数据recv_peek，直到遇到\n
	}

	return -1;
}

void do_service(int conn)
{
	char recvbuf[1024];
	while (1)
	{
		memset(recvbuf, 0, sizeof(recvbuf));
		int ret = readline(conn, recvbuf, 1024);
		if (ret == -1)
			ERR_EXIT("readline");
		if (ret == 0)
		{
			printf("client close\n");
			break;
		}

		fputs(recvbuf, stdout);
		//将读到的数据再发送，发送没有对\n处理
		writen(conn, recvbuf, strlen(recvbuf));
	}
}

int main(void)
{
	int listenfd;
	if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		/*	if ((listenfd = socket(PF_INET, SOCK_STREAM, 0)) < 0)*/
		ERR_EXIT("socket");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8001);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/*servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");*/
	/*inet_aton("127.0.0.1", &servaddr.sin_addr);*/

	int on = 1;
	//设置socket 地址复用
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");

	if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;

	pid_t pid;
	while (1)
	{
		if ((conn = accept(listenfd, (struct sockaddr*) &peeraddr, &peerlen))
				< 0)
			ERR_EXIT("accept");

		printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr),
				ntohs(peeraddr.sin_port));

		pid = fork();
		if (pid == -1)
			ERR_EXIT("fork");
		if (pid == 0)
		{
			close(listenfd);
			do_service(conn);
			exit(EXIT_SUCCESS);
		} else
			close(conn);
	}

	return 0;
}
