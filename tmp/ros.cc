//支持detect download
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <signal.h>

#define SERV_PORT 80
#define SERV_IP "10.0.120.106"

char* resp = "HTTP/1.1 200 OK\r\nContent-Length: 10240000\r\nServer: JSP3/2.0.14\r\nDate: Tue, 11 Jun 2019 07:15:53 GMT\r\nContent-Type: application/octet-stream\r\nConnection: keep-alive\r\nExpires: Tue, 09 Jun 2020 15:51:28 GMT\r\nAge: 24779\r\nAccept-Ranges: bytes\r\nCache-Control: max-age=31536000\r\nAccess-Control-Allow-Origin: *\r\nx-bce-debug-id: p2GFL1MuyyoedTRf5601JmnksQVaZ7FleSpIa3r4sKfdxr78IKEEBLhyPZrVFDN26AZK/bskfFbshoHPTJpOfw==\r\nx-bce-request-id: be4ca52d-24eb-4bf1-9f36-5628a69e1f52\r\nOhc-Response-Time: 1 0 0 0 0 0\r\nOhc-File-Size: 19095912\r\nQY-H-M: HIT\r\nOhc-Storage-MD5: 149bb34799806045701a088b4699729a\r\n\r\n";
char* resp_hdr = "HTTP/1.1 200 OK\r\nContent-Length: 10240000\r\nServer: JSP3/2.0.14\r\nDate: Tue, 11 Jun 2019 07:15:53 GMT\r\nContent-Type: application/octet-stream\r\nConnection: keep-alive\r\nExpires: Tue, 09 Jun 2020 15:51:28 GMT\r\nAge: 24779\r\nAccept-Ranges: bytes\r\nCache-Control: max-age=31536000\r\nAccess-Control-Allow-Origin: *\r\nx-bce-debug-id: p2GFL1MuyyoedTRf5601JmnksQVaZ7FleSpIa3r4sKfdxr78IKEEBLhyPZrVFDN26AZK/bskfFbshoHPTJpOfw==\r\nx-bce-request-id: be4ca52d-24eb-4bf1-9f36-5628a69e1f52\r\nOhc-Response-Time: 1 0 0 0 0 0\r\nOhc-File-Size: 19095912\r\nQY-H-M: HIT\r\nOhc-Storage-MD5: 149bb34799806045701a088b4699729a\r\n\r\n";


int main2(void)
{
    int sfd, cfd;
    int len, i;
    char buf[BUFSIZ], clie_IP[BUFSIZ];
    //char resp[1024] = "HTTP/1.1 200 OK\r\nX-Content-Length: 1024000\r\nContent-Length: 10240000\r\nContent-Length-private: 3\r\n\r\n";
    char resp[8*1024] = {0};

    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;

    char* file_buf = (char*)malloc(1024 * sizeof(char));
    memset(file_buf, 0x9, 1024);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    signal(SIGPIPE, SIG_IGN);

    bzero(&serv_addr, sizeof(serv_addr));           //将整个结构体清零
    serv_addr.sin_family = AF_INET;                 //选择协议族为IPv4
    //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //监听本地所有IP地址
    serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);  //监听本地所有IP地址
    serv_addr.sin_port = htons(SERV_PORT);          //绑定端口号    
    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    int r = bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(r == -1) {
        printf("bind err: %s\n", strerror(errno));
        return 0;
    }

    listen(sfd, 128);

    printf("wait for client connect ...\n");

    clie_addr_len = sizeof(clie_addr_len);
start:
    while(1) {
        int LENGTH = 821560;
        cfd = accept(sfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
        printf("------>client IP:%s\tport:%d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), ntohs(clie_addr.sin_port));
        memset(buf, 0x0, BUFSIZ);
        read(cfd, buf, BUFSIZ);
        //printf("req buf: %s\n", buf);
        if(1) {
            char* buf = (char*)malloc(strlen(resp_hdr) + LENGTH); //10K + 1.6K
            memset(buf, 0x1, strlen(resp_hdr) + LENGTH);
            memcpy(buf, resp_hdr, strlen(resp_hdr));
            int ret1 = write(cfd, buf, strlen(resp_hdr) + LENGTH);
            printf("ret:------------------> %d\n", ret1);
            
            close(cfd);
        }
    }
end:
    close(sfd);
//goto start;
    return 0;
}

int main(void)
{
    int sfd, cfd;
    int len, i;
    char buf[BUFSIZ], clie_IP[BUFSIZ];
    char resp_1024[1024] = "HTTP/1.1 206 OK\r\nX-Content-Length: 1024000\r\nContent-Range: bytes 0-1023/10240000\r\nContent-Length-private: 3\r\nContent-Length: 1024\r\n\r\n";

    struct sockaddr_in serv_addr, clie_addr;
    socklen_t clie_addr_len;

    char* file_buf = (char*)malloc(1024 * sizeof(char));
    memset(file_buf, 0x9, 1024);

    sfd = socket(AF_INET, SOCK_STREAM, 0);
    signal(SIGPIPE, SIG_IGN);

    bzero(&serv_addr, sizeof(serv_addr));           //将整个结构体清零
    serv_addr.sin_family = AF_INET;                 //选择协议族为IPv4
    //serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  //监听本地所有IP地址
    serv_addr.sin_addr.s_addr = inet_addr(SERV_IP);  //监听本地所有IP地址
    serv_addr.sin_port = htons(SERV_PORT);          //绑定端口号    
    int reuse = 1;
    setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    int r = bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if(r == -1) {
        printf("bind err: %s\n", strerror(errno));
        return 0;
    }

    listen(sfd, 128);

    printf("wait for client connect ...\n");

    clie_addr_len = sizeof(clie_addr_len);
    while(1) {
        cfd = accept(sfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
        printf("------>client IP:%s\tport:%d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), ntohs(clie_addr.sin_port));
        memset(buf, 0x0, BUFSIZ);
        read(cfd, buf, BUFSIZ);
        //printf("req buf: %s\n", buf);
        if(strstr(buf, "Range") != NULL) {
            write(cfd, resp_1024, strlen(resp_1024)); 
            int ret1 = 0;
            ret1 = write(cfd, file_buf, 1024); 
            if(ret1 == 1024) {
                printf("detecting success, send 1024\n");
            } else {
                printf("detecting err, send !1024\n");
                close(cfd);
                goto end;
            }
            close(cfd);
        } else if(strstr(buf, "DOWNFILE") != NULL) {
            int ret = write(cfd, resp, strlen(resp)); 
            int sum = 0;
            int ret1 = 0;
            while(1) {
                ret1 = write(cfd, file_buf, 1024); 
                if(ret1 == -1) {
                    printf("download write return -1, errno: %s\n", strerror(errno));
                    close(cfd);
                    goto end;
                }
                sum += ret1;
                if(sum >=  10240000) break;
            }
            if(sum == 10240000) {
                printf("downloading success\n");
            }
            close(cfd);
        }
    }
end:
    close(sfd);
    return 0;
}
