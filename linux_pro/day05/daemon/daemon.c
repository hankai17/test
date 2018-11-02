#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(void)
{
    pid_t pid;

    pid = fork();
    if (pid > 0) {
        exit(1);
    } 

    setsid();

    chdir("/home/itcast/0706_Linux");

    umask(0022);

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while (1);

    return 0;
}
