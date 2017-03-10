#include<stdio.h>
#include<sys/types.h>
#include<regex.h>

int main()
{
    int status, i;
    int cflags = REG_EXTENDED; //以功能更加强大的扩展正则表达式的方式进行匹配
    regmatch_t pmatch[1]; //里面存放起始位置 和 结束位置
    const size_t nmatch = 1;
    regex_t reg; //用来存放编译后的正则表达式，它的成员re_nsub 用来存储正则表达式中的子正则表达式的个数，子正则表达式就是用圆括号包起来的部分表达式
    const char * pattern = "^\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*.\\w+([-.]\\w+)*$";
    //const char * pattern = "^(.+)@(.+)$";
    char * buf ="david19842003@gmail.com";

    regcomp(&reg,pattern,cflags); //编译正则表达式
    status = regexec(&reg,buf,nmatch,pmatch,0);
    if(status == REG_NOMATCH)
        printf("No Match\n");
    else if(status == 0)
    {
        printf("Match:\n");
        for(i=pmatch[0].rm_so; i<pmatch[0].rm_eo; ++i)
            putchar(buf[i]);
        printf("\n");
    }
    regfree(&reg);
    return 0;
}

