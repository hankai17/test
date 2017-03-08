#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

#define LEN 4

int main(void)
{
    int fd = 0;
    int ret, len;

    fd = open("dict.txt", O_RDWR|O_CREAT, 0644);
    if (fd == -1) 
    {
        perror("open error");
        exit(1);
    }
    printf("open success");
    write(fd,"hello world1!\n",15);
    write(fd,"hello world2!\n",14);
    write(fd,"hello world3!\n",14);
    close(fd);
    //fd = open("dict.txt", O_RDWR|O_CREAT, 0644);
    //write(fd,"hello world4!\n",14);
    //close(fd);


    return 0;
}

