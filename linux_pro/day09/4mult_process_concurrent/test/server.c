#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>

#include "wrap.h"

#define SERV_PORT 8888

void do_child(int signum)
{
    while (waitpid(0, NULL, WNOHANG) > 0);

    return ;
}

int main(void)
{
    int lfd, cfd;
    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;
    pid_t pid;
    char buf[BUFSIZ], clie_IP[32];
    int ret, i;

    lfd = Socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERV_PORT);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    Listen(lfd, 128);

    printf("Server Accept......\n");

    clie_addr_len = sizeof(clie_addr);
    while (1) {
        cfd = Accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);

        printf("client IP:%s, port:%d\n", 
                inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
                ntohs(clie_addr.sin_port));

        pid = fork();
        if (pid < 0) {
            perror("fork error");
            exit(1);
        } else if (pid == 0) {
            close(lfd);
            break;
        } else {
            signal(SIGCHLD, do_child);
            close(cfd);
        }
    }

    if (pid == 0) {
        while (1) {
            ret = Read(cfd, buf, sizeof(buf));
            if (ret == 0) {
                close(cfd);
                return 0;
            } else if (ret == -1) {
                if (errno == EINTR) {
                    printf("Interrapt by signal\n");
                    break;
                } else {
                    perror("read error");
                    break;
                }
            }

            for (i = 0; i < ret; i++)
                buf[i] = toupper(buf[i]);
            write(cfd, buf, ret);
            write(STDOUT_FILENO, buf, ret);
        }
        close(cfd);
    }

    return 0;
}
