#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

typedef void (*sighandler_t)(int);

void do_wait(int signo)
{
    pid_t pid = 0;
    int status;
    printf("----------------%d\n", pid);
    while ((pid = waitpid(0, &status, WNOHANG)) > 0) 
    {
        if (WIFEXITED(status))
        printf("child exit with %d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
        printf("child killed by %d\n", WTERMSIG(status));
    }
}

int main(void)
{
    pid_t pid;

    int i ;
    for (i= 0; i < 10; i++) 
    {
        pid = fork();
        if (pid == 0)
        break;
    }

    if (i < 10) 
    {  //child

     printf("-----die\n");
     sleep(1);
     return i;

    } 
    else 
    {
        struct sigaction act;

        act.sa_handler = do_wait;
        sigemptyset(&act.sa_mask);
        act.sa_flags = 0;
        sigaction(SIGCHLD, &act, NULL);

        while (1);
    }

    return 0;
}


