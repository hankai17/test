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
    //fd = open("hellommap", O_RDWR|O_CREAT, 0644);
    if (fd == -1) {
        perror("open error");
        exit(1);
    }

/*
    len = lseek(fd, 3, SEEK_SET);   
    write(fd, "e", 1);              
    printf("The length of file = %d\n", len);
    ret = ftruncate(fd, LEN);
    if (ret == -1) {
        perror("ftruncate error");
        exit(1);
    }
*/
    //char *p = (char*)mmap(NULL, 20, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 4096);
    char *p = (char*)mmap(NULL, 20, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) 
    {
        perror("---------mmap error");
        exit(1);
    }
    close(fd);

    strcpy(p, "hehehehehhehe\n");

    printf("%s\n", p);

    ret = munmap(p, LEN);
    if (ret == -1) 
    {
        perror("munmap error");
        exit(1);
    }
    printf("--------------munmap over\n");

    //close(fd);

    return 0;
}
