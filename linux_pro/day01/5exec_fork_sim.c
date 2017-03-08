#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    char *argvv[] = {"ls", "-l","-a", NULL};

    pid_t pid = fork();
    if (pid == 0) 
    {
        //execl("/bin/ls", "ls", "-l","-a", NULL);
        execv("/bin/ls", argvv);
        perror("execlp");
        exit(1);
    } 
    else if (pid > 0) 
    {
        sleep(1);
        printf("parent\n");
    }

    return 0;
}
