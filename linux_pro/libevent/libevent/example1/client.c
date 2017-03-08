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
    //while(1)
    {
        //printf("******\n");
        sleep(2);
        write(fd,"hello world",strlen("hello world"));
        //sleep(20);
    }
} 
void main_loop(evutil_socket_t fd1)
{
    struct event_base *base = event_base_new();
    ev1 = event_new(base, fd1, EV_WRITE, cb_func,(char*)"Writing event");
    event_add(ev1, NULL);
    event_base_dispatch(base);
}
int main(void)
{
    const char *fifo = "event.fifo";
    int socket1;
    socket1 = open("event.fifo", O_WRONLY);
    if (socket1 == -1) 
    {
        perror("open socket 2 erorr");
        exit(1);
    }
    main_loop(socket1);
    return 0;
}
