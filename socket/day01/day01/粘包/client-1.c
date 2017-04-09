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

//定义自定义封装包
struct packet
{
	int len;
	char buf[1024];
};

ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count; //剩下需要读取的数据的个数
	ssize_t nread; //成功读取的个数
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
	size_t nleft = count;
	ssize_t nwritten;
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

int main(void)
{

	int sock;
	//定义socket
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	// 定义结构体
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8001);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	/*int connect(int sockfd, const struct sockaddr *addr,
	 socklen_t addrlen);
	 */
	if (connect(sock, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("connect");

	struct packet sendbuf;
	struct packet recvbuf;
	memset(&sendbuf, 0, sizeof(sendbuf));
	memset(&recvbuf, 0, sizeof(recvbuf));
	int n;
	/*
	 * char *fgets(char *s, int size, FILE *stream);
	 * */
	while (fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin) != NULL)
	{
		//获取字符串的大小
		n = strlen(sendbuf.buf);
		//将n转换为网络字节存储
		sendbuf.len = htonl(n);
		//将获取的包发送
		writen(sock, &sendbuf, 4 + n);

		int ret = readn(sock, &recvbuf.len, 4);
		if (ret == -1)
			ERR_EXIT("read");
		else if (ret < 4)
		{
			printf("client close\n");
			break;
		}
		//将转换为本地字节存储
		n = ntohl(recvbuf.len);
		ret = readn(sock, recvbuf.buf, n);
		if (ret == -1)
			ERR_EXIT("read");
		else if (ret < n)
		{
			printf("client close\n");
			break;
		}

		fputs(recvbuf.buf, stdout);
		memset(&sendbuf, 0, sizeof(sendbuf));
		memset(&recvbuf, 0, sizeof(recvbuf));
	}

	close(sock);

	return 0;
}
