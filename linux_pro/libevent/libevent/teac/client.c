#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    const char *fifo = "event.fifo";
    int fd;
    char *str = "hello event!";
    fd = open(fifo, O_WRONLY);
    if (fd == -1) {
        perror("open fd erorr");
        exit(1);
    }
    while (1) {
        write(fd, str, strlen(str));
        sleep(1);
    }
    close(fd);
        return 0;
}
