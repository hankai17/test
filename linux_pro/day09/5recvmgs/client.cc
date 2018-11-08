#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <errno.h>

#define DEFAULT_PORT 8080
#define MAXLINE 4096
#define MAX_NAME_LEN 4095
#define MAX_MMSG_NUM 10

//send fd to server with sendmsg
int write_message(int socket_fd, int socket_num, int32_t* socket_array, int32_t* data) {
    struct iovec iov[1];
    char buf[CMSG_SPACE(sizeof(int) * MAX_MMSG_NUM)];
    struct msghdr msg;
    {
        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_flags = 0;
        msg.msg_iov = iov;
        //msg.msg_iov = 0;
        msg.msg_iovlen = sizeof(iov) / sizeof(iov[0]);
        //msg.msg_iovlen = 0;
        msg.msg_control = buf;
        //msg.msg_control = NULL;
        msg.msg_controllen = sizeof(buf);
        //msg.msg_controllen = 0;

        iov[0].iov_base = const_cast<int32_t*>(data);
        iov[0].iov_len = sizeof(data[0]) * MAX_MMSG_NUM;

        struct cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;
        cmsg->cmsg_len = CMSG_LEN(sizeof(int32_t) * socket_num);
        int32_t* fdptr = reinterpret_cast<int32_t*>(CMSG_DATA(cmsg));
        for (uint32_t i = 0; i < socket_num; i++) {
            fdptr[i] = static_cast<int32_t>(socket_array[i]);
        }
        msg.msg_controllen = CMSG_SPACE(sizeof(int32_t) * socket_num);
    }

    //send info to client
    while (true) {
        errno = 0;
        const ssize_t n = sendmsg(socket_fd, &msg, 0);
        if (-1 == n) {
            std::cout << "send message error " << strerror(errno) << std::endl;
            if (EINTR == errno) {
                continue;
            }
            return errno;
        }
        std::cout << "sendmsg success n = " << n << std::endl;
        break;
    }
    return 0;
}

int main(int argc, char** argv) {
    int socket_fd;
    int num = 0;
    int rec_len = 0;
    //struct sockaddr_in servaddr;// net conn
    struct sockaddr_un servaddr;//local conn
    char recvline[4096];
    char sendline[4096];
    char buff[MAXLINE];

    // apply socket_fd
#if 0
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cout << "create socket error" << std::endl;
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(DEFAULT_PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //transfer ip
#else
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        std::cout << "create socket error" << std::endl;
        exit(1);
    }
    servaddr.sun_family = AF_UNIX;
    snprintf(servaddr.sun_path, MAX_NAME_LEN, "%s", "server_socket");
#endif
    int ret = 0;
    //conn
    ret = connect(socket_fd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if (ret < 0) {
        std::cout << "connect error " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "send message to server" << std::endl;
#if 0
    std::string msg = "hello server!";
    strcpy(sendline, msg.c_str());
    int time = 3;
    while(time > 0) {
        ret = send(socket_fd, sendline, strlen(msg.c_str()), 0);
        if (ret < 0) {
            std::cout << "send msg error" << strerror(errno) << std::endl;
            continue;
        }
        std::cout << "send again" << std::endl;
        time--;
    }
    rec_len = recv (socket_fd, buff, MAXLINE, 0);
    if (rec_len < 0) {
        std::cout << "recv msg error" << strerror(errno) << std::endl;
    }
    buff[rec_len] = '\0';
    std::cout << "received " << buff << std::endl;
#else
    int32_t array[MAX_MMSG_NUM];
    for (int i = 0; i < MAX_MMSG_NUM; i++) {
        array[i] = socket(AF_UNIX, SOCK_STREAM, 0);
    }

    int32_t data[MAX_MMSG_NUM];
    for (int i = 0; i < MAX_MMSG_NUM; i++) {
        data[i] = i;
        std::cout << "the data adn fd is " << data[i] << " " <<array[i] << std::endl;
    }

    write_message(socket_fd, MAX_MMSG_NUM, array, data);

#endif
    close(socket_fd);
}

