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
//自定义封包的结构体
struct packet
{
	int len; //自定义的包头，包含了包的大小
	char buf[1024]; //包存放的数据
};

//1一次全部读走 //2次读完数据 //出错分析 //对方已关闭
//思想：tcpip是流协议，不能保证1次读操作，能全部把报文读走，所以要循环读指定长度的数据。
//按照count大小读数据，
//若读取的长度ssize_t<count 说明读到了一个结束符，对方已关闭。

//@ssize_t：返回读的长度 若ssize_t<count 读失败失败
//@buf：接受数据内存首地址
//@count：接受数据长度
ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count; //定义剩余没有读取的个数
	ssize_t nread; //读取的个数
	char *bufp = (char*) buf; //将参数接过来

	while (nleft > 0) //当剩余需要读取的个数>0
	{
		//成功读取的个数小于0,则判断出错的原因
		if ((nread = read(fd, bufp, nleft)) < 0)
		{
			//如果errno被设置为EINTR为被信号中断，如果是被信号中断继续，
			//不是信号中断则退出。
			if (errno == EINTR)
				continue;
			return -1;
		} else if (nread == 0) //若对方已关闭

			return count - nleft;

		bufp += nread; //将 字符串指针向后移动已经成功读取个数的大小。
		nleft -= nread; //需要读取的个数=需要读取的个数-以及成功读取的个数
	}

	return count;
}

//1一次全部读走 //2次读完数据 //出错分析 //对方已关闭
//思想：tcpip是流协议，不能1次把指定长度数据，全部写完 
//按照count大小写数据
//若读取的长度ssize_t<count 说明读到了一个结束符，对方已关闭。
//@ssize_t：返回写的长度 -1失败
//@buf：待写数据首地址
//@count：待写长度

ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count; //需要写入的个数
	ssize_t nwritten; //已经成功写入的个数
	char *bufp = (char*) buf; //接参数

	while (nleft > 0) //如果需要写入的个数>0
	{
		//如果写入成功的个数<0 判断是否是被信号打断
		if ((nwritten = write(fd, bufp, nleft)) < 0)
		{
			//信号打断，则继续
			if (errno == EINTR)
				continue;
			return -1;
		}
		//需要写入的数据个数>0
		//如果成功写入的个数为0 则继续
		else if (nwritten == 0)
			continue;
		//将bufp指针向后移动已经
		bufp += nwritten;
		//剩余个数
		nleft -= nwritten;
	}

	return count;
}

void do_service(int conn)
{
	//定义了自定封包结构体
	struct packet recvbuf;
	//
	int n;
	while (1)
	{
		//清空结构体
		memset(&recvbuf, 0, sizeof(recvbuf));
		//读取包的头4字节
		int ret = readn(conn, &recvbuf.len, 4); //读包头 4个字节
		if (ret == -1)
			ERR_EXIT("read");
		//如果读取的个数小于4,则客服端已经关闭
		else if (ret < 4)
		{
			printf("client close\n");
			break;
		}
		//将网络数据转换为本地数据结构，比如网络数据为大端，而本地数据为小端
		n = ntohl(recvbuf.len);
		//根据包头里包含的大小读取数据
		ret = readn(conn, recvbuf.buf, n); //根据长度读数据
		if (ret == -1)
			ERR_EXIT("read");
		//如果读取的数据的大小小于封包包头中包的大小，那么客服端已经关闭
		else if (ret < n)
		{
			printf("client close\n");
			break;
		}
		//将数据打印出。
		fputs(recvbuf.buf, stdout);
        //将接受到的数据再直接发出去。
		writen(conn, &recvbuf, 4 + n);  //注意写数据的时候，多加4个字节
	}
}

int main(void)
{
	int listenfd;  //定义了监听socket的文件描述符
	//创建socket  TCP 协议
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
	//设置地址复用
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
		ERR_EXIT("setsockopt");
	//绑定
	if (bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
	//监听
	if (listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");

	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;

	pid_t pid;
	while (1)
	{
		//接受已经链接的socket
		if ((conn = accept(listenfd, (struct sockaddr*) &peeraddr, &peerlen))
				< 0)
			ERR_EXIT("accept");

		printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr),
				ntohs(peeraddr.sin_port));
		//新建子进程
		pid = fork();
		if (pid == -1)
			ERR_EXIT("fork");
		if (pid == 0)
		{  //子进程
		   //子进程不需要监听socket，关闭监听的socket
			close(listenfd);
			do_service(conn);
			exit(EXIT_SUCCESS);
		} else
			close(conn);
	}

	return 0;
}
