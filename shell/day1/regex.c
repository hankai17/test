#include <sys/types.h>
#include <regex.h>
#include <stdio.h>

int main(int argc, char ** argv)
{
    if (argc != 3) {
        printf("Usage: %s RegexString Text\n", argv[0]);
        return 1;
    }

    const char * pregexstr = argv[1];
    const char * ptext = argv[2];
    regex_t oregex;                 //编译后的结构体
    int nerrcode = 0;
    char szerrmsg[1024] = {0};      //保存错误信息的数组
    size_t unerrmsglen = 0;

    if ((nerrcode = regcomp(&oregex, pregexstr, REG_EXTENDED|REG_NOSUB)) == 0) {    //编译正则表达式, 扩展正则
        if ((nerrcode = regexec(&oregex, ptext, 0, NULL, 0)) == 0)  {               //执行匹配, 不保存匹配的返回值
            printf("%s matches %s\n", ptext, pregexstr);
            regfree(&oregex);
            return 0;
        }
    }

    unerrmsglen = regerror(nerrcode, &oregex, szerrmsg, sizeof(szerrmsg));          //正则编译错误,存szerrmsg中错误描述

    unerrmsglen = unerrmsglen < sizeof(szerrmsg) ? unerrmsglen : sizeof(szerrmsg) - 1;  //错误信息较长特殊情况

    szerrmsg[unerrmsglen] = '\0';
    printf("Regex error Msg: %s\n", szerrmsg);

    regfree(&oregex);

    return 1;                   //非正常退出.
}

