#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>


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

    lfd = socket(AF_INET, SOCK_STREAM, 0);

    Bind(lfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    listen(lfd, 18);

    clie_addr_len = sizeof(clie_addr);
    cfd = accept(lfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
    printf("client IP:%s, port:%d\n", 
            inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
            ntohs(clie_addr.sin_port));

    while (1) {
        ret = read(cfd, buf, sizeof(buf));

        for(i = 0; i < ret; i++)
            buf[i] = toupper(buf[i]);

        write(cfd, buf, ret);
    }

    close(lfd);
    close(cfd);

    return 0;
}
