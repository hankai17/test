/*************************************************************************
  > File Name: 5.cpp
  > Author: guaz
  > Mail: hankai17@126.com 
  > Created Time: 2017-02-28 09:51:56
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int get_name_by_pid(pid_t pid, char* name)
{
    int fd;
    char buf[1024] = {0};
    snprintf(buf, 1024, "/proc/%d/cmdline", pid);
    if ((fd = open(buf, O_RDONLY)) == -1)
        return -1;
    read(fd, buf, 1024);
    strncpy(name, buf, 1023);
    return 0;
}
int check_user()
{
    char *password="123",user[5]="";
    printf("password:"); 
    scanf("%s",user);
    if(!strcmp(password,user)) 
    {printf("success!\n");return 0;} 
    else 
    {printf("Error!\n");return -1;}
}

int main(void)
{
    char name[1024]; 
    pid_t ppid = getppid(); 
    if (get_name_by_pid(ppid, name)) //name=/proc/%d/cmdline
        return -1;

    if (strcmp(name,"bash")==0||strcmp(name,"init")==0)
    { 
        pid_t ppid = getppid(); 
        if (get_name_by_pid(ppid, name))
            return -1;
        if (strcmp(name,"bash")==0||strcmp(name,"init")==0)
        { 
            check_user();
        }
        else if (strcmp(name,"gdb")==0 || strcmp(name,"strace")==0 || strcmp(name,"ltrace") == 0)
            printf("Traced!\n");
        else
            printf("Unknown! Maybe traced!\n");
    }
    else if (strcmp(name,"gdb")==0 || strcmp(name,"strace")==0 || strcmp(name,"ltrace") == 0)
        printf("Traced!\n");
    else
        printf("Unknown! Maybe traced!\n");

    return 0;
}

