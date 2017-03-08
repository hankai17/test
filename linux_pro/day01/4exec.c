#include <stdio.h>
#include <unistd.h>

int main()
{
    //execl("/bin/ls","ls","-l","-a",NULL);
    //execlp("ls","ls","-l","-a",NULL);
    //char *argv[]={"ls","-l","-a",NULL};
    char *argv[]={"wrong",NULL};

    execv("./wrong",argv);
    perror("error:");
    printf("bye bye!\n");
    return 0;
}

