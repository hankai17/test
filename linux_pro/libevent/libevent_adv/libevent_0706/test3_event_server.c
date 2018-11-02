#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include <event2/event.h>

void read_callback(evutil_socket_t fd, short events, void *arg)
{
    char buf[256] = {0};
    int len = 0;

    len = read(fd, buf, sizeof(buf));
    if (len == 0) {
        perror("remote close");
        return;
    }
    else if (len < 0) {
        perror("read");
        return;
    }
    else {
        buf[len] = '\0';
        printf("read buf [%s]\n", buf);
    }

}

int main(int argc, char *argv[])
{

    const char *fifo = "event.fifo";

    int fd;

    unlink(fifo);

    if (mkfifo(fifo, 0644)) {
        perror("mkfifor error");
        exit(1);
    }


    fd = open(fifo, O_RDONLY);
    if (fd == -1) {
        perror("open fd erorr");
        exit(1);
    }


    //创建base
    struct event_base *base = NULL;
    base = event_base_new();



    //将事件添加到base中
    //创建一个事件， 同时是一个 初始化事件  fd callback  
    struct event *ev;
    ev = event_new(base, fd, EV_READ|EV_PERSIST, read_callback, NULL);

    //将ev事件 由初始化 ---》未决 让base去监听
    event_add(ev, NULL);


    //循环监听
    event_base_dispatch(base);

    //释放base
    event_base_free(base);
    close(fd);


	return 0;
}
