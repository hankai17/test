#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    int ret;
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork err:");
        exit(1);
    }
    else if(pid == 0)
    {
        printf("I am child,pid=%u,ppid=%u\n",getpid(),getppid());
        int a = 5;
        a =a/0;
        sleep(3);
        //exit(77);
        return 100;
    }
    else
    {
        int status;
        //wait(NULL);
        ret = wait(&status);
        if(WIFEXITED(status))
        {
            printf("child quit with %d\n",WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status))
        {
            printf("child kill by %d\n",WTERMSIG(status));
        }
    }
    printf("byebye\n");
    return 0;
}

