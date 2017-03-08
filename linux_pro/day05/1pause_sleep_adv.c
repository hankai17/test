#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

void catch_sigalrm(int signo)
{
    ;
}

unsigned int mysleep(unsigned int seconds)
{
    int ret;
    struct sigaction act, oldact;

    act.sa_handler = catch_sigalrm;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    ret = sigaction(SIGALRM, &act, &oldact);
    if (ret == -1) 
    {
        perror("sigaction error");
        exit(1);
    }

    alarm(seconds);
    //.....................................
    ret = pause();  //主动挂起  等信号

    if (ret == -1 && errno == EINTR) 
    {
        printf("pause sucess\n");
    }

    ret = alarm(0);
    sigaction(SIGALRM, &oldact, NULL);  //回复SIGALRM信号旧有的处理方式。

    return ret;
}

int main(void)
{
    while (1) 
    {
        mysleep(1);
        printf("-----------------\n");
    }

    return 0;
}
