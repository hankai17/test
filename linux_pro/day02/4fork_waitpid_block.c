#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork err:");
        exit(1);
    }
    else if(pid == 0)
    {
        printf("I am child,pid=%u,ppid=%u\n",getpid(),getppid());
        //int a = 5;
        //a =a/0;
        sleep(10);
        exit(77);
        return 100;
    }
    else
    {
        int status;
        printf("Begin waitpid\n");
        int ret = waitpid(-1,&status,0);//只回收资源不关心死亡原因
        printf("end waitpid");
        if(WIFEXITED(status))
        {
            printf("child quit with %d\n",WEXITSTATUS(status));
        }
        if(WIFSIGNALED(status))
        {
            printf("child kill by %d\n",WTERMSIG(status));
        }
        ret = wait(NULL);
        printf("ret ===%d\n",ret);
    }
    printf("byebye\n");
    return 0;
}

