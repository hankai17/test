/*************************************************************************
> File Name: myshell.c
> Author:red devil Manchester 
> Mail:867614535@qq.com 
> Created Time: 2016年09月25日 星期日 12时29分58秒
************************************************************************/
#include <stdio.h>  
#include <string.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <sys/wait.h>  
#define DEBUG 1  
#define BUFSIZE 1024  
#define CMDNUMBER 100  
#define SHELL_CD 1  
#define SHELL_FORK 2  
#define SHELL_EMPTY 3  
int read_char(char *str);  
int parse_args(char *args[], char *arg);  
int isquit(char *args);  
char *shell_prompt(char *promptbuf);  
int dealarg(char *arg);  
int shell_cd(char *args[]);  
int shell_fork(char *args[]);  
int get_cmdnum(char *cmd);  
int mutifork(char *args[]);  
int main(int argc, char *argv[])  
{  
    char arg[BUFSIZE];  
    ssize_t readbytes;  
    char str_prompt[BUFSIZE + 1];  
    while(1)
    {  
        fprintf(stdout, "%s", shell_prompt(str_prompt));  
        fflush(stdout);//must!! fprintf+fflush is a couple  
        //void *memset(void *s, int c, size_t n);  
        memset(arg, 0, sizeof(arg));  
        //ssize_t read(int fd, void *buf, size_t count);  
        if((readbytes = read(STDIN_FILENO, arg, BUFSIZE)) < 0)//can read "enter"
        {  
            fprintf(stderr, "read failed: %s\n", strerror(errno));  
            exit(1);  
        }  
        if(arg[readbytes - 1] == '\n')
        { /* clear '\n' at end of arg */  
            arg[readbytes - 1] = '\0';  
        }   
        if(isquit(arg) == 1)
        { /* check quit or not */  
            break;  
        }  
        //int dealarg(char *arg);  
        dealarg(arg);  
    }  
    return 0;  
}  
/* * parse args * note that: args[n] and arg shared memory! args arg are input*/  
// q point to--------->before:"ls -l | grep a | wc  -l"  after:"ls0-l0|0grep0a0|0wc0 -l0"
// p point to--------->q:0xaa11 0xaa14 0xaa14 ... 
int parse_args(char *args[], char *arg)  
{  
    char **p;  
    char *q;  
    int ch;  
    int count;  
    q = arg;  // q point to--------->before:"ls -l | grep a | wc  -l"  after:"ls0-l0|0grep0a0|0wc0 -l0"
    p = args; // p point to--------->q:0xaa11 0xaa14 0xaa14 ... NULL
    count = 0;  
    while(*q != '\0')
    {  
        //int read_char(char *arg);  
        while((ch = read_char(q)) == ' ')
        { /* skip space */  
            q++;  
        }  
        *p++ = q++;  
        count++;  
        ch = read_char(q);  
        while(ch != ' ' && ch != '\0')
        { /* find first space after word */  
            q++;  
            ch = read_char(q);  
        }  
        if(ch != '\0')
        {   
            *q++ = '\0';  
            ch = read_char(q);  
        }  
    }  
    return count;  
}  
/* * filter string */  
int read_char(char *str)  
{  
    char filter[] = " \t\n";  
    char *p;  
    int flag;       /* flag 1 return ' ', 0 return *str */  
    flag = 0;  
    p = filter;  
    while(*p != '\0')
    {  
        if(*str == *p)
        {  
            flag = 1;  
            break;  
        }  
        p++;  
    }  
    if(flag == 1)
    {  
        return ' ';  
    }
    else
    {  
        return *str;  
    }  
}  
/* * print shell prompt */  
char *shell_prompt(char *promptbuf)  
{  
    char tmpbuf[BUFSIZE];  
    //void *memset(void *s, int c, size_t n);  
    memset(promptbuf, 0, BUFSIZE);  
    memset(tmpbuf, 0, sizeof(tmpbuf));  
    //char *getcwd(char *buf, size_t size);  
    if(getcwd(tmpbuf, sizeof(tmpbuf)) == NULL)
    {  
        fprintf(stderr, "%s:%d: getcwd failed: %s\n",   
                __FILE__, __LINE__, strerror(errno));  
        exit(1);  
    }  
    //int snprintf(char *str, size_t size, const char *format, ...);  
    snprintf(promptbuf, BUFSIZE, "%s$ ", tmpbuf);  
    return promptbuf;  
}  
/* * if args == quit */  
int isquit(char *arg)  
{  
    //int strcmp(const char *s1, const char *s2);  
    if(strcmp(arg, "quit") == 0){  
        return 1;  
    }else{  
        return 0;  
    }  
}  
/* * Receive message and deal something */  
int  dealarg(char *arg)  
{  
    int cmdnum;  
    char *args[CMDNUMBER];  
    int argnum;  
    //int parse_args(char *args[], char *arg);   
    argnum = parse_args(args, arg);  
    args[argnum] = NULL;  
    #if DEBUG  
    int i;  
    fprintf(stdout, "argnum = %d\n", argnum);  
    for(i = 0; i < argnum; i++)
    {  
        fprintf(stdout, "[%d]%s\n", i, args[i]);  
    }  
    #endif  
    //int get_cmdnum(char *cmd)  
    cmdnum = get_cmdnum(args[0]);//either cd or fork just two conditions!!!!  
    switch(cmdnum)
    {  
        case SHELL_EMPTY:  
        break;  
        case SHELL_CD:  
        //int shell_cd(char *args[])  
        shell_cd(args);  
        break;  
        case SHELL_FORK:  
        //int shell_fork(char *args[])  
        shell_fork(args);  
        break;  
        default:  
        fprintf(stdout, "%s:%d: getcmd failed\n", __FILE__, __LINE__);  
        break;  
    }  
    return 0;  
}  
/* * fork and called exec */  
int shell_fork(char *args[])  
{  
    pid_t pid[CMDNUMBER];  
    int status;  
    int fork_num;  
    char **p;       /* point args */  
    char *q;        /* point *args */  
    /* get numbers of child process*/  
    fork_num = 1;  
    p = args;  
    while(*p != NULL)
    {  
        q = *p;  
        while(*q != '\0')
        {  
            if(*q == '|')
            {  
                fork_num++;  
            }  
            q++;  
        }  
        p++;  
    }  
    #if DEBUG  
    fprintf(stdout, "fork_num = %d\n", fork_num);  
    #endif  
    /* case: child process number is one */  
    if(fork_num < 2)
    {  
        //pid_t fork(void);  
        if((pid[0] = fork()) < 0)
        {  
            /* error */  
            perror("fork");  
            exit(1);  
        }
        else if(pid[0] == 0)
        {  
            /* child process */  
            //int execvp(const char *file, char *const argv[]);  
            if(execvp(args[0], args) < 0)
            {  
                perror("");  
                exit(1);  
            }  
            exit(0);  
        }  
    }  
    /* parent process */  
    //pid_t waitpid(pid_t pid, int *status, int options);  
    if(fork_num < 2)
    {  
        waitpid(pid[0], &status, 0);  
    }
    else
    {  
        //int mutifork(char *args[])  
        status = mutifork(args);  
    }  
return status;  
}  
/* * likes shell's cd */  
int shell_cd(char *args[])  
{  
    char buf[BUFSIZE];  
    memset(buf, 0, BUFSIZE);  
    if(args[1][0] != '/' && args[1][0] != '.')//just open current path eg: cd currentfile
    {  
        //char *getcwd(char *buf, size_t size);  
        if(getcwd(buf, BUFSIZE) == NULL)
        {  
            fprintf(stderr, "%s:%d: getcwd failed: %s\n", __FILE__,   
                    __LINE__, strerror(errno));  
            return -1;  
        }  
        strncat(buf, "/", BUFSIZE - strlen(buf));  
    }  
    //char *strncat(char *dest, const char *src, size_t n);  
    strncat(buf, args[1], BUFSIZE - strlen(buf)); // cd /home/hk/currentfile or cd ./current
    #if DEBUG  
    fprintf(stdout, "%s\n", buf);  
    #endif  
    //int chdir(const char *path);  
    if(chdir(buf) == -1)
    {  
        fprintf(stderr, "%s:%d: chdir failed: %s\n", __FILE__,   
                __LINE__, strerror(errno));  
    }  
    return 0;  
}  
/* * Change cmd to int, according to shell.h */  
int get_cmdnum(char *cmd)  
{  
    if(cmd == NULL) return SHELL_EMPTY;  
    if(strcmp(cmd, "cd") == 0) return SHELL_CD;  
    return SHELL_FORK;  
}  
int mutifork(char *args[])  
{  
    int pipefd[CMDNUMBER][2];  
    pid_t pid[CMDNUMBER];  
    int i, j;  
    int count;  
    int status;  
    char **arg_child[CMDNUMBER];  
    char **p;  
    char ***q;  
    /* parse and split args to child arg */  
    count = 0;  
    p = args; q = arg_child;  
    while(*p != NULL && p != NULL){  
        *q++ = p;  
        count++;  
        while(*p != NULL && strcmp(*p, "|") != 0){  
            p++;  
        }  
        if(*p != NULL){  
            *p++ = NULL;  
        }     
    }  
    *q = NULL;  
    #if DEBUG           /* check child args */  
    fprintf(stdout, "----------------------------------------\n");  
    fprintf(stdout, "count = %d\n", count);  
    q = arg_child; i = 0;  
    while(*q != NULL){  
        p = *q++;  
        while(*p != NULL){  
            fprintf(stdout, "[%d]%s\n", i, *p++);  
        }  
        i++;  
    }  
    #endif  
    /* fork count child process */  
    for(i = 0; i < count; i++){  
        /* init pipe file descriptor */  
        if(pipe(pipefd[i]) < 0){ /* FIXME: excess one */  
                                perror("pipe");  
                                return -1;  
                               }  
        /* fork child i */  
        if((pid[i] = fork()) < 0){  
            fprintf(stderr, "%s:%d: fork() failed: %s\n", __FILE__,  
                    __LINE__, strerror(errno));  
            return -1;  
        }else if(pid[i] == 0){  
            /* child i */  
            //int dup2(int oldfd, int newfd);  
            if(i == 0){ /* the first child */  
                       close(pipefd[i][0]); /* close curr process read */  
                       if(dup2(pipefd[i][1], STDOUT_FILENO) < 0){  
                           perror("dup2 failed");  
                           return -1;  
                       }  
                      }else if(i == count - 1){ /* the last child */  
                                               for(j = 0; j < i - 1; j++){ /* close unuse pipefd */  
                                                                          close(pipefd[j][1]);  
                                                                          close(pipefd[j][0]);  
                                                                         }  
                                               close(pipefd[j][1]); /* close prev process end of write */  
                                               close(pipefd[i][0]); /* close curr process end of read */  
                                               if(dup2(pipefd[j][0], STDIN_FILENO) < 0){  
                                                   perror("dup2 failed");  
                                                   return -1;  
                                               }  
                                              }else{  
                                                  for(j = 0; j < i - 1; j++){ /* close unuse pipefd */  
                                                                             close(pipefd[j][1]);  
                                                                             close(pipefd[j][0]);  
                                                                            }  
                                                  close(pipefd[j][1]); /* close prev process end of write */  
                                                  close(pipefd[i][0]); /* close curr process end of read */  
                                                  if(dup2(pipefd[j][0], STDIN_FILENO) < 0){  
                                                      perror("dup2 failed");  
                                                      return -1;  
                                                  }  
                                                  if(dup2(pipefd[i][1], STDOUT_FILENO) < 0){  
                                                      perror("dup2 failed");  
                                                      return -1;  
                                                  }  
                                              }  
            //int execvp(const char *file, char *const argv[]);  
            if(execvp(arg_child[i][0], arg_child[i]) < 0){  
                fprintf(stderr, "%s:%d: fork() failed: %s\n", __FILE__,  
                        __LINE__, strerror(errno));  
                exit(1);  
            }  
            exit(0);  
            /* child process exit */  
}  
}  
    /* parent process */  
    for(i = 0; i < count; i++){  
        /* close all pipe file descriptor */  
        close(pipefd[i][0]);  
        close(pipefd[i][1]);      
    }  
    for(i = 0; i < count; i++){  
        //pid_t waitpid(pid_t pid, int *status, int options);  
        waitpid(pid[i], &status, 0);  
        if(status != 0){  
            return status;  
        }  
    }  
    return status;  
}  

