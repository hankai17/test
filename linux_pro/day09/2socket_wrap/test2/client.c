#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "wrap.h"

#define SERV_PORT 7666
#define SERV_IP "127.0.0.1"

int main(void)
{
    int sfd;
    int ret;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ];

    sfd = Socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);  

    Connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) {
        fgets(buf, sizeof(buf), stdin);
        printf("-----------------1----\n");
        Write(sfd, buf, strlen(buf));
        printf("-----------------2----\n");
        ret = Read(sfd, buf, sizeof(buf));
        printf("-----------------3----\n");
        Write(STDOUT_FILENO, buf, ret);
    }

    Close(sfd);

    return 0;
}
