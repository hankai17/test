#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    int fd=open("ps.out",O_WRONLY|O_CREAT,0644);
    if(fd == -1)
    {
        perror("open err:");
        exit(1);
    }
    dup2(fd,STDOUT_FILENO);
    execlp("ps","ps","aux",NULL);
    perror("error:");
    printf("bye bye!\n");
    return 0;
}

