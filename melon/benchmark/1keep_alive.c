#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define BUFSIZE 10240
#define DestIp "127.0.0.1"
#define DestPort 8001
//#define Req "GET http://127.0.0.1:80/index.html HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: Close\r\n\r\n"
#define Req "GET http://127.0.0.1:8001/1.html HTTP/1.1\r\nHost: 127.0.0.1\r\nConnection: Keep-Alive\r\n\r\n"
#define ReqLen sizeof(Req)

int main(int argc, char *argv[]) 
{
    ssize_t i;
    int nRequestLen;

    char strResponse[BUFSIZE]={0};
    char strRequest[BUFSIZE]={0};


    int sockfd, numbytes;
    struct sockaddr_in dest_addr; /* connector's address information */

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    dest_addr.sin_family = AF_INET; /* host byte order */
    dest_addr.sin_port = htons(DestPort); /* short, network byte order */
    dest_addr.sin_addr.s_addr = inet_addr(DestIp);

    int optval;
    socklen_t optlen = sizeof(optval);
    if(getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
        perror("set keep alive");
        exit(1);
    }

    /* Create and setup the connection */
    if (connect(sockfd, (struct sockaddr *)&dest_addr,sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }

    /* Send the request */
    strncpy(strRequest, Req,ReqLen);
    nRequestLen = ReqLen;
    if (write(sockfd,strRequest,nRequestLen) == -1) {
        perror("write");
        exit(1);
    }

    /* Read in the response */
    while(1) {
        //i = read(sockfd,strResponse,BUFSIZE-1);
        i = read(sockfd,strResponse,1);
        printf("i:%d\n",i);
        if(0 >= i){
            break;
        }
        strResponse[i]='\0';
        printf(strResponse);
        i = read(sockfd,strResponse,1);
        printf("i:%d\n",i);
        break;
    }
    for (int i = 0; i < 20; i++) {
        sleep(1);
        printf("sleep...%d\n",i + 1);
    }

    /* Close the connection */
    close(sockfd);
}
