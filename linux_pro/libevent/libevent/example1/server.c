#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <event2/event.h>
#include <fcntl.h>
#include <unistd.h>
struct event *ev1;
void cb_func(evutil_socket_t fd, short what, void *arg)
{
    int ret=0;
    printf("5s is coming\n");
    const char *data = arg;
    char buf[1024]={0};
    read(fd,buf,sizeof(buf));
    printf("read:%s\n",buf);
    return;
} 
void main_loop(evutil_socket_t fd1)
{
    struct timeval five_seconds = {5,0};
    struct event_base *base = event_base_new();
    ev1 = event_new(base, fd1,EV_READ|EV_PERSIST,cb_func,(char*)"Reading event");
    //ev1 = event_new(base, fd1,EV_READ,cb_func,(char*)"Reading event");
    //event_add(ev1,NULL);
    event_add(ev1, &five_seconds);
    event_base_dispatch(base);
}
int main(void)
{
    const char *fifo = "event.fifo";
    int socket1;
    unlink(fifo);
    if (mkfifo(fifo, 0755)) 
    {
        perror("mkfifor error");
        exit(1);
    }
    socket1 = open(fifo,O_RDWR);
    if (socket1 == -1) {
        perror("open socket 1 erorr");
        exit(1);
    }
    main_loop(socket1);
    //while(1);
    return 0;
}
