#include <stdio.h>
#include <stdlib.h>

int main()
{
    printf("begin\n");
    pid_t pid;
    int i = 0;
    for(i = 0 ; i < 5; i ++)
    {
        pid=fork();
        if(pid == -1)
        {
            perror("fork err:");
            exit(1);
        }
        else if(pid == 0)//如果是子进程直接结束循环
        {
            break;
            printf("I am %d child,pid=%u,ppid=%u\n",i+1,getpid(),getppid());
        }
    }
    if(i < 5)//代表子进程
    {
        //sleep(i);
        printf("I am %d child,pid=%u,will die\n",i+1,getpid());
    }
    else
    {
        
        while((pid=wait(NULL)) > 0 )
        {
            printf("release child %u\n",pid);
        }
    }
    return 0;
}

