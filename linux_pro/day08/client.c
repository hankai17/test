#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERV_IP "127.0.0.1"
#define SERV_PORT 9527

int main(void)
{
    int sfd, len;
    struct sockaddr_in serv_addr;
    char buf[BUFSIZ]; 

    sfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&serv_addr, sizeof(serv_addr)); 
    serv_addr.sin_family = AF_INET;      
    inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);   
    serv_addr.sin_port = htons(SERV_PORT);                    

    connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1) 
    {
        fgets(buf, sizeof(buf), stdin);
        write(sfd, buf, strlen(buf));       
        len = read(sfd, buf, sizeof(buf));
        write(STDOUT_FILENO, buf, len);
    }

    close(sfd);

    return 0;
}

