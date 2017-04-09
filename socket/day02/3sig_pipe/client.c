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
        char *bufp = (char*)buf;

        while (nleft > 0)
        {
                if ((nread = read(fd, bufp, nleft)) < 0)
                {
                        if (errno == EINTR)
                                continue;
                        return -1;
                }
                else if (nread == 0)
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
        char *bufp = (char*)buf;

        while (nleft > 0)
        {
                if ((nwritten = write(fd, bufp, nleft)) < 0)
                {
                        if (errno == EINTR)
                                continue;
                        return -1;
                }
                else if (nwritten == 0)
                        continue;

                bufp += nwritten;
                nleft -= nwritten;
        }

        return count;
}

//读数据，但不把数据缓冲区清空
//@ssize_t返回值：返回缓冲区数据的长度 -1失败
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
        while (1)
        {
                int ret = recv(sockfd, buf, len, MSG_PEEK);
                if (ret == -1 && errno == EINTR)
                        continue;
                return ret;
        }
}

//功能：按行读取文件，只要遇到\n就，读走数据，返回，
//@buf 接收数据内存首地址
//@maxline 接收数据内存最大值
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
        int ret;
        int nread;
        char *bufp = buf;
        int nleft = maxline;
        while (1)
        {
        		//读数据，但不把数据缓冲区清空
        		//成功：ret是报文的长度
                ret = recv_peek(sockfd, bufp, nleft);
                if (ret < 0)
                        return ret;
                else if (ret == 0)
                        return ret;

                nread = ret;
                int i;
                for (i=0; i<nread; i++)
                {
                	//读数据，但不把数据缓冲区清空，避免了一个字节一个字节的读数据
                	//先利用recv的MSG_PEEK功能，预读数据，然后查找\n
                	//根据\n的位置，根据指定长度，再真正的读数据
                    if (bufp[i] == '\n')
                    {
                    		//将数据从缓存区读走
                            ret = readn(sockfd, bufp, i+1);
                            if (ret != i+1)
                                    exit(EXIT_FAILURE);

                            return ret;
                    }
                }

				//若数据长度nread > 缓冲区最大长度maxline 退出
                if (nread > nleft)
                        exit(EXIT_FAILURE);

				//若没有\n 说明消息还没有结束，不是完整的一条消息，就把这些数据也读到buf缓冲区中。
				//依此循环，直到遇到\n,把整个一行数据，全部读完，放入buf中
				//bufp记录了每次需追加的位置
                nleft -= nread;
                ret = readn(sockfd, bufp, nread);
                if (ret != nread)
                        exit(EXIT_FAILURE);

                bufp += nread; //bufp每次跳到追加的末尾
        }

        return -1;
}

int main(void)
{
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");

	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8001);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("connect");

	struct sockaddr_in localaddr;
	socklen_t addrlen = sizeof(localaddr);
	//获取本地的地址 注意是已连接以后的套接字
	if (getsockname(sock, (struct sockaddr*)&localaddr, &addrlen) < 0)
		ERR_EXIT("getsockname");

	printf("本机的ip=%s port=%d\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));


	char sendbuf[1024] = {0};
	char recvbuf[1024] = {0};
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{
		//写数据（本身带有\n）所以不需要再单独加 \n 
		writen(sock, sendbuf, strlen(sendbuf));

		//按行读数据
		int ret = readline(sock, recvbuf, sizeof(recvbuf));
        if (ret == -1)
                ERR_EXIT("readline");
        else if (ret == 0)
        {
                printf("client close\n");
                break;
        }

		fputs(recvbuf, stdout);
		memset(sendbuf, 0, sizeof(sendbuf));
		memset(recvbuf, 0, sizeof(recvbuf));
	}

	close(sock);
	
	return 0;
}
//起服务器 客户端 
//若先关闭服务器 看网络状态都 阻塞到了fin_wait2 和 close_wait
//就是说 对端没有往下推进 即客户端没有调用close函数 
//
//因为客户端阻塞在161行 没有机会write readline犯错 无法退出 调用close 
//所以手动敲一个键就可以了
//需要注意的是 write是可以写的 读的时候会出错 ret返回0 再break出来
//为什么可以写？？？？？？？？
