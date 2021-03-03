#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUMBER 1024
#define BUFFER_SIZE 1024
struct fds {
        int epollfd;
        int sockfd;
};

int setnonblocking(int fd) {
        int old_option = fcntl(fd, F_GETFL);
        int new_option = old_option | O_NONBLOCK;
        fcntl(fd, F_SETFL, new_option);
        return old_option;
}


void addfd(int epollfd, int fd, bool oneshot) {
        epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET;
        if (oneshot) {
                event.events |= EPOLLONESHOT;
        }
        epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
        setnonblocking(fd);
}

void reset_oneshot(int epollfd, int fd) {
        epoll_event event;
        event.data.fd = fd;
        event.events = EPOLLIN | EPOLLET | EPOLLONESHOT;
        epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void* worker(void *arg) {
        int sockfd = ((fds*)arg)->sockfd;
        int epollfd = ((fds*)arg)->epollfd;
        pthread_t pid = pthread_self();

        printf("start new thread %u to recv data on fd: %d\n", pid, sockfd);
        char buf[BUFFER_SIZE];
        memset(buf, '\0', BUFFER_SIZE);

        while(1) {
                //sleep(100); // et模式下 就绪后不做io 也会触发接下来事件?!!!
                int ret = recv(sockfd, buf, BUFFER_SIZE-1, 0);
                //int ret = recv(sockfd, buf, 1, 0);
                if (ret == 0) {
                        close(sockfd);
                        printf("foreiner closed the connection\n");
                        break;
                }
                else if (ret < 0) {
                        if (errno == EAGAIN) {
                                reset_oneshot(epollfd, sockfd);
                                printf("EAGAIN read later\n");
                                break;
                        }
                }
                else {
                        buf[ret] = '\0';
                        printf("thread %u get content: %s\n", pid, buf);
                        printf("thread %u about to sleep\n", pid);
                        sleep(5);
                        printf("thread %u back from sleep\n", pid);
                }
        }
        //printf("end thread %u receiving data on fd: %d\n", pid, sockfd);

}

int main(int argc, char *argv[]) {
        if (argc <= 1) {
                printf("usage: %s port_number [ip_address]\n", basename(argv[0]));
                return 1;
        }
        int port = atoi(argv[1]);

        int ret = 0;
        sockaddr_in address;
        bzero(&address, sizeof(address));
        address.sin_family = AF_INET;
        if (argc >= 3) {
                const char *ip =argv[2];
                inet_pton(AF_INET, ip, &address.sin_addr);
        }
        else {
                address.sin_addr.s_addr = INADDR_ANY;
        }
        address.sin_port = htons(port);
        int listenfd = socket(PF_INET, SOCK_STREAM, 0);
        assert(listenfd >= 0);

        ret = bind(listenfd, (sockaddr*)&address, sizeof(address));
        assert(ret != -1);

        ret = listen(listenfd, 5);
        assert(ret != -1);

        epoll_event events[MAX_EVENT_NUMBER];
        int epollfd = epoll_create(5);
        assert(epollfd != -1);

        addfd(epollfd, listenfd, false);

        while(1) {
                int ret = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
                if (ret < 0) {
                        printf("epoll failure\n");
                        break;
                }

                for (int i=0; i<ret; i++) {
                        int sockfd = events[i].data.fd;
                        if (sockfd == listenfd) {
                                sockaddr_in client_address;
                                socklen_t client_addrlength = sizeof(client_address);
                                int connfd = accept(listenfd, (sockaddr*)&client_address,
                                                        &client_addrlength);

                                addfd(epollfd, connfd, false); // if false that will trigger when there has event
                                // false则一直监听in事件 只要有in事件到来就会不断的起新线程
                                //true即oneshot  事件到来后则触发一次 因为这个fd上的这个事件会被清除（主要是为了避免多个线程读数据时候相互干扰）
                                printf("new connection is added to epollfd\n");
                        }
                        else if (events[i].events & EPOLLIN) {
                                pthread_t thread;
                                fds fds_for_new_worker;
                                fds_for_new_worker.epollfd = epollfd;
                                fds_for_new_worker.sockfd = sockfd;
                                // new thread
                                pthread_create(&thread, NULL, worker,
                                                (void*)&fds_for_new_worker);
                        }
                        else {
                                printf("something else happened\n");
                        }
                }
        }

        close(listenfd);
        return 0;

}
