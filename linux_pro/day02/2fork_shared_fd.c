#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int main()
{
    int fd = open("test.txt",O_WRONLY|O_CREAT,0644);
    if(fd < 0)
    {
        perror("open err");
        exit(1);
    }
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork err:");
        exit(1);
    }
    else if(pid == 0)
    {
        printf("I am child,pid=%u,ppid=%u\n",getpid(),getppid());
        write(fd,"child\n",6);
    }
    else
    {
        printf("I am parent,pid=%u,ppid=%u\n",getpid(),getppid());
        sleep(1);
        write(fd,"parent\n",7);
    }
    return 0;
}

