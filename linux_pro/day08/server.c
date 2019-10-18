#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SERV_PORT 9527

int main(void)
{
    int sfd, cfd;
    int len;
    char buf[BUFSIZ], clie_IP[BUFSIZ];

    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;     
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);       

    bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));


    listen(sfd, 64);

    printf("wait for client connect ...\n");

    clie_addr_len = sizeof(clie_addr_len);
    cfd = accept(sfd, (struct sockaddr *)&clie_addr, &clie_addr_len);      

    printf("client IP:%s\tport:%d\n", 
           inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), 
           ntohs(clie_addr.sin_port));

    while (1) 
    {
        len = read(cfd, buf, sizeof(buf));
        //write(STDOUT_FILENO, buf, len);
        //for (i = 0; i < len; i++)
        //buf[i] = toupper(buf[i]);
        write(cfd, buf, len); 
    }

    close(sfd);
    close(cfd);

    return 0;
}
