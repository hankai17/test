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

/*
Content-Range: bytes 2312094-2434841/2434842
Content-Length: 122748
Server: Byte-nginx
Content-Type: video/mp4
Connection: keep-alive
Date: Sat, 10 Aug 2019 01:26:53 GMT
x-oss-request-id: 5D4E1D5D129E2621AFE866D4
Accept-Ranges: bytes
x-oss-object-type: Normal
x-oss-hash-crc64ecma: 17923091968925461273
x-oss-storage-class: Standard
Expires: Mon, 09 Sep 2019 01:22:29 GMT
Cache-Control: max-age=15552000
x-oss-expiration: expiry-date="Mon, 10 Aug 2020 00:00:00 GMT", rule-id="6f36f853-7a57-4ccb-80ee-9704db11a39d"
Content-MD5: horJPgErWTFvNc24osmv1Q==
x-oss-server-time: 6
Ali-Swift-Global-Savetime: 1565400413
X-Swift-SaveTime: Sat, 10 Aug 2019 01:26:53 GMT
X-Swift-CacheTime: 2592000
Via: cache13.l2cn1793[0,206-0,H], cache25.l2cn1793[0,0], cache25.l2cn1793[0,0], cache2.cn1295[32,206-0,M], cache20.cn1295[34,0],cache06.wxcm, http/1.1 cache06.wxcm (bdcache [cRs f ]),cache01.wxcm
Age: 1034
X-Cache: MISS TCP_MISS dirn:-2:-2 mlen:1255194
X-Swift-SaveTime: Sat, 10 Aug 2019 01:27:26 GMT
X-Swift-CacheTime: 2591967
EagleId: 701919a815654004466084649e
x-request-ip: 221.178.202.229
x-tt-trace-tag: id=5
Bd-Status: TCP_HIT
Access-Control-Allow-Credentials: true
Access-Control-Allow-Headers: range
Access-Control-Allow-Methods: GET,HEAD,POST,OPTIONS
Access-Control-Max-Age: 3600
Accept-Ranges: bytes
NETEAST-CACHE: DISK_HIT
NETEAST-URLHASH: EF82D32D313FAF44FFFE259028D86434E613E957
NETEAST-COMBIN: R_HIT
NETEAST-SITEID: 277
NETEAST-INFOHASH: 0C04CAE49E4C19A2DD85BA4EDD74C76E0F43E36B
Access-Control-Allow-Origin: *
Timing-Allow-Origin: *
*/

const char* resp_hdr = "HTTP/1.1 200 OK\r\nContent-Length:  1024000\r\nServer: Byte-nginx\r\nContent-Length-private: 3\r\nX-Content-Length: 1024000\r\nContent-Type: video/mp4\r\nConnection: keep-alive\r\nDate: Sat, 10 Aug 2019 01:26:53 GMT\r\nx-oss-request-id: 5D4E1D5D129E2621AFE866D4\r\nAccept-Ranges: bytes\r\nx-oss-object-type: Normal\r\nx-oss-hash-crc64ecma: 17923091968925461273\r\nx-oss-storage-class: Standard\r\nExpires: Mon, 09 Sep 2019 01:22:29 GMT\r\nCache-Control: max-age=15552000\r\nx-oss-expiration: expiry-date=\"Mon, 10 Aug 2020 00:00:00 GMT\", rule-id=\"6f36f853-7a57-4ccb-80ee-9704db11a39d\"\r\nContent-MD5: horJPgErWTFvNc24osmv1Q==\r\nx-oss-server-time: 6\r\nAli-Swift-Global-Savetime: 1565400413\r\nX-Swift-SaveTime: Sat, 10 Aug 2019 01:26:53 GMT\r\nX-Swift-CacheTime: 2592000\r\nVia: cache13.l2cn1793[0,206-0,H], cache25.l2cn1793[0,0], cache25.l2cn1793[0,0], cache2.cn1295[32,206-0,M], cache20.cn1295[34,0],cache06.wxcm, http/1.1 cache06.wxcm (bdcache [cRs f ]),cache01.wxcm\r\nAge: 1034\r\nX-Cache: MISS TCP_MISS dirn:-2:-2 mlen:1255194\r\nX-Swift-SaveTime: Sat, 10 Aug 2019 01:27:26 GMT\r\nX-Swift-CacheTime: 2591967\r\nEagleId: 701919a815654004466084649e\r\nx-request-ip: 221.178.202.229\r\nx-tt-trace-tag: id=5\r\nBd-Status: TCP_HIT\r\nAccess-Control-Allow-Credentials: true\r\nAccess-Control-Allow-Headers: range\r\nAccess-Control-Allow-Methods: GET,HEAD,POST,OPTIONS\r\nAccess-Control-Max-Age: 3600\r\nAccept-Ranges: bytes\r\nNETEAST-CACHE: DISK_HIT\r\nNETEAST-URLHASH: EF82D32D313FAF44FFFE259028D86434E613E957\r\nNETEAST-COMBIN: R_HIT\r\nNETEAST-SITEID: 277\r\nNETEAST-INFOHASH: 0C04CAE49E4C19A2DD85BA4EDD74C76E0F43E36B\r\nAccess-Control-Allow-Origin: *\r\nTiming-Allow-Origin: *\r\n\r\n";

int main(void)
{
    int sfd, cfd;
    int len, i;
    char buf[BUFSIZ], clie_IP[BUFSIZ];
    //char resp[1024] = "HTTP/1.1 200 OK\r\nX-Content-Length: 1024000\r\nContent-Length: 10240000\r\nContent-Length-private: 3\r\n\r\n";
    char resp[8*1024] = {0};
    printf("costume resp_hdr_len: %d\n", strlen(resp_hdr));
    memcpy(resp, resp_hdr, strlen(resp_hdr));
    printf("costume resp_hdr: %s\n", resp_hdr);

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
        cfd = accept(sfd, (struct sockaddr *)&clie_addr, &clie_addr_len);
        printf("------>client IP:%s\tport:%d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), ntohs(clie_addr.sin_port));
        memset(buf, 0x0, BUFSIZ);
        read(cfd, buf, BUFSIZ);
        //printf("req buf: %s\n", buf);
        if(1) {
            int ret = write(cfd, resp, 1024);
            printf("ret1: %d, will sleeping 2s...\n", ret);
            sleep(200);
            ret = write(cfd, resp + 1024, strlen(resp + 1024));
            printf("ret2: %d, send all\n", ret);
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
//goto start;
    return 0;
}

int main1(void)
{
    int sfd, cfd;
    int len, i;
    char buf[BUFSIZ], clie_IP[BUFSIZ];
    //char resp[1024] = "HTTP/1.1 206 Partial Content\r\nContent-Range: bytes 0-10/11\r\nContent-Length: 11\r\n\r\n12345678901";
    char resp[1024] = "HTTP/1.1 200 OK\r\nX-Content-Length: 1024000\r\nContent-Length: 10240000\r\nContent-Length-private: 3\r\n\r\n";
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
