#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "wrap.h"

#define SERV_PORT 6666

int main(int argc, char *argv[])
{
    int i, j, n, nready;

    int maxfd = 0;
    int listenfd, connfd;
    char buf[BUFSIZ];         /* #define INET_ADDRSTRLEN 16 */

    struct sockaddr_in clie_addr, serv_addr;
    socklen_t clie_addr_len;

    fd_set rset, allset;                            /* rset 读事件文件描述符集合 allset用来暂存 */

    listenfd = Socket(AF_INET, SOCK_STREAM, 0);  
    int opt = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family= AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port= htons(SERV_PORT);
    Bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    Listen(listenfd, 128);

    maxfd = listenfd;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);                                  /* 构造select监控文件描述符集 */

    while (1) {   
        rset = allset;                                          /* 每次循环时都从新设置select监控信号集 */
        nready = select(maxfd+1, &rset, NULL, NULL, NULL);
        if (nready < 0)
            perr_exit("select error");

        if (FD_ISSET(listenfd, &rset)) {                        /* 说明有新的客户端链接请求 */

            clie_addr_len = sizeof(clie_addr);
            connfd = Accept(listenfd, (struct sockaddr *)&clie_addr, &clie_addr_len);       /* Accept 不会阻塞 */

            FD_SET(connfd, &allset);                            /* 向监控文件描述符集合allset添加新的文件描述符connfd */
            FD_CLR(listenfd, &rset);                           

            if (maxfd < connfd)
                maxfd = connfd;
        } 

        for (i = listenfd+1; i <= maxfd; i++) {                 /* 检测哪个clients 有数据就绪 */

            if (FD_ISSET(i, &rset)) {
                if ((n = Read(i, buf, sizeof(buf))) == 0) {    /* 当client关闭链接时,服务器端也关闭对应链接 */
                    Close(i);
                    FD_CLR(i, &allset);                        /* 解除select对此文件描述符的监控 */

                } else if (n > 0) {

                    for (j = 0; j < n; j++)
                        buf[j] = toupper(buf[j]);
                    Write(i, buf, n);
                    Write(STDOUT_FILENO, buf, n);
                }
            }
        }
    }

    Close(listenfd);

    return 0;
}

