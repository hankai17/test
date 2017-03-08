#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int var = 100;

int main()
{
    printf("begin\n");
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork err:");
        exit(1);
    }
    else if(pid == 0)//子进程
    {
        sleep(1);
        printf("I am child,pid=%u,ppid=%ui,var=%d\n",getpid(),getppid(),var);
        //printf("var_addr:%d\n",(int)(&var));
    }
    else
    {
        printf("I am parent,pid=%u,ppid=%u,var=%d\n",getpid(),getppid(),var);
        var = 51;
        sleep(1);
        printf("after,I am parent,pid=%u,ppid=%u,var=%d\n",getpid(),getppid(),var);
        //printf("var_addr:%d\n",(int)&var);

    }
    return 0;
}

