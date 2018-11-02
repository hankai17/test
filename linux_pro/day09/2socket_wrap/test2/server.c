#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "wrap.h"

#define SERV_PORT 7666

int main(void)
{
    int lfd, cfd, ret, i;
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    char buf[BUFSIZ], clie_IP[BUFSIZ];

    bzero(&serv_addr, sizeof(serv_addr));       //memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT); 
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    Listen(lfd, 18);

    clie_addr_len = sizeof(clie_addr);
    cfd = Accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
    printf("client IP:%s, port:%d\n", 
            inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(clie_addr.sin_port));

    while (1) {
        ret = Read(cfd, buf, sizeof(buf));
        if (ret == -1) {
            if (errno == EAGAIN) {
                sleep(1);
                printf("NONBLOCK ...\n");
            } else {
                perror("read error");
                exit(1);
            }
        } else if (ret == 0) {
            printf("The other side colsed...\n");
            break;
        }

        for(i = 0; i < ret; i++)
            buf[i] = toupper(buf[i]);

        Write(cfd, buf, ret);
    }

    Close(lfd);
    Close(cfd);

    return 0;
}
