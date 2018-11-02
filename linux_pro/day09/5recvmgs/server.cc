#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <errno.h>

#define DEFAULT_PORT 8088
#define MAXLINE 4096
#define MAX_NAME_LEN 4095
#define MAX_MMSG_NUM 10

int receive_message(int socket_fd, int* socket_num, int* sockets) {
    struct iovec iov[1];
    int32_t utm[MAX_MMSG_NUM];

    char buf[CMSG_SPACE(sizeof(int) * MAX_MMSG_NUM)];
    struct msghdr msg;
    {
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_flags = 0;
        msg.msg_iov = iov;
        //msg.msg_iov = 0;
        msg.msg_iovlen = sizeof(iov) /sizeof(iov[0]);
        //msg.msg_iovlen = 0;
        msg.msg_control = buf;
        //msg.msg_control = NULL;
        msg.msg_controllen = sizeof(buf);
        //msg.msg_controllen = 0;
        msg.msg_iov[0].iov_base = utm;
        msg.msg_iov[0].iov_len = sizeof(utm);
    }

    errno = 0;
    const int n = recvmsg(socket_fd, &msg, MSG_WAITALL);
    std::cout << "receive finish n=" << n << "sizeof(utm)=" << sizeof(utm) << std::endl;
    struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
    {
        if (n <= 0) {
            std::cout << "receive message faild, return " << strerror(errno) << std::endl;
            return errno;
        }
        if(0 != (msg.msg_flags & (MSG_TRUNC | MSG_CONFIRM))) {
            std::cout << "receive message truncated, flags=" << msg.msg_flags << std::endl;
            return errno;
        }
    }
    *socket_num = (cmsg->cmsg_len - CMSG_LEN(0)) / sizeof(int32_t);
    int32_t* fd_arr = reinterpret_cast<int32_t*>(CMSG_DATA(cmsg));
    for (int32_t i = 0; i < *socket_num; i++) {
        sockets[i] = fd_arr[i];
        std::cout << "the data recv is " << utm[i] <<" " << sockets[i] << std::endl;
    }
    return 0;
}

int main(int argc, char** argv) {
    int socket_fd;
    int connect_fd;
    int num = 0;
    //struct sockaddr_in servaddr;//net conn
    struct sockaddr_un servaddr;//local conn
    char buff[4096];

#if 0
    //init the socket
        std::cout << "create socket error" << std::endl;
    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        exit(1);
    }

    // init the in struct
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//set the server ip
    servaddr.sin_port = htons(DEFAULT_PORT);//set the listion port
#else
    //unlink("server_socket");
    if ((socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        std::cout << "create socket error" << std::endl;
        exit(1);
    }

    servaddr.sun_family = AF_UNIX;
    snprintf(servaddr.sun_path, MAX_NAME_LEN, "%s", "server_socket");
#endif
    //bind the struct to the socket
    if ( bind(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1 ) {
        std::cout << "bind socket error: " << strerror(errno) <<std::endl;
        exit(1);
    }

    //listion the socket 10 is the max conn num
    if ( listen(socket_fd, 10) == -1 ) {
        std::cout << "listen error" << std::endl;
    }

    std::cout << "start to listen 8080 port" << std::endl;
    connect_fd = accept(socket_fd, (struct sockaddr*)NULL, NULL);
    if ( connect_fd == -1 ) {
        std::cout << "accept socket error" << strerror(errno) << std::endl;
    }
#if 0
    while(true) {
        num = recv(connect_fd, buff, MAXLINE, 0);
        if (num < 0) {
            std::cout << "receive error" << strerror(errno) << std::endl;
        }
        buff[num] = '\0';
        std::cout << "receive from client " << buff << std::endl;

        if ( send(connect_fd, "we have received your message", 29, 0) == -1 ) {
            std::cout << "send error" << strerror(errno) << std::endl;
        }
        sleep(3);
    }
#else
    int socket_num = 0;
    int socket_array [MAX_MMSG_NUM];
    receive_message(connect_fd, &socket_num, socket_array);
#endif
}

