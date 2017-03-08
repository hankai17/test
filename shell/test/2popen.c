/*************************************************************************
	> File Name: popen.c
	> Author:hankai123 
	> Mail:867614535@qq.com 
	> Created Time: 2016年09月16日 星期五 10时41分16秒
 ************************************************************************/

#include<stdio.h>

int main()
{
    FILE* fp;
    fp=popen("ls -l|sort -n -k 5 -r","w");
    //while(1);
    pclose(fp);
    return 0;
}
