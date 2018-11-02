#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <event2/event.h>
#include <fcntl.h>
#include <unistd.h>

struct event *ev1, *ev2;

void cb_func(evutil_socket_t fd, short what, void *arg)
{
    const char *data = arg;
    printf("Got an event on socket %d:%s%s%s%s [%s]\n",
            (int) fd,
            (what&EV_TIMEOUT) ? " timeout" : "",
            (what&EV_READ) ? " read" : "",
            (what&EV_WRITE) ? " write" : "",
            (what&EV_SIGNAL) ? " signal" : "",
            data);
} 

void cb_func_2(evutil_socket_t fd, short what, void *arg)
{
    char *str = (char*) arg;
    printf("write, add event %s\n", str);

    //非未决 --> 未决
    event_add(ev2, NULL);
} 

void cb_func_3(evutil_socket_t fd, short what, void *arg)
{
    struct event_base * base= (struct event_base*)arg;
    printf("write, add event\n");


    printf("dump base\n");


    FILE*fp = NULL;

    fp = fopen("event_base.txt", "a");
    if (fp == NULL) {
        perror("fopen error");
        return;
    }

    event_base_dump_events(base, fp);

    fclose(fp);

    
} 

void main_loop(evutil_socket_t fd1, evutil_socket_t fd2)
{
    struct timeval five_seconds = {5,0};

    //创建一个base
    struct event_base *base = event_base_new();

    //创建一个初始化状态的事件 ev1
    ev1 = event_new(base, fd1, EV_TIMEOUT|EV_READ|EV_PERSIST
            , cb_func,
            (char*)"Reading event");

    //创建一个初始化状态的事件 ev2
    ev2 = event_new(base, fd2, EV_WRITE, cb_func_3, base);


    //将ev1 由 初始化 ---> 未决状态  让base去监控该事件绑定的fd的状态变化 , 同时指定超时事件
    event_add(ev1, &five_seconds);
    //将ev2 由 初始化 ---> 未决状态  让base去监控该事件绑定的fd的状态变化 
    event_add(ev2, NULL);

    //base 循环监控事件状态
    event_base_dispatch(base);
}


int main(void)
{
    const char *fifo = "event.fifo";

    int socket1, socket2;

    unlink(fifo);

    if (mkfifo(fifo, 0644)) {
        perror("mkfifor error");
        exit(1);
    }


    socket1 = open(fifo, O_RDONLY|O_NONBLOCK);
    if (socket1 == -1) {
        perror("open socket 1 erorr");
        exit(1);
    }

    socket2 = open(fifo, O_WRONLY|O_NONBLOCK);
    if (socket2 == -1) {
        perror("open socket 2 erorr");
        exit(1);
    }


    main_loop(socket1, socket2);
    
    
    return 0;
}
