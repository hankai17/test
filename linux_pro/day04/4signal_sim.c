#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>

void do_sig(int signum)
{
    if (signum == SIGINT)
    printf("Hi, SIGINT, how do you do !\n");
    else if (signum == SIGQUIT)
    printf("SIGQUIT-------------, how do you do !\n");
}

int main(void)
{
    if (signal(SIGINT, do_sig) == SIG_ERR) 
    {
        perror("signal");
        exit(1);
    }
    if (signal(SIGQUIT, do_sig) == SIG_ERR) 
    {
        perror("signal");
        exit(1);
    }

    while (1) 
    {
        printf("---------------------\n");
        sleep(1);
    }

    return 0;
}
