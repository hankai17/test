/*************************************************************************
	> File Name: sort.c
	> Author:hankai123 
	> Mail:867614535@qq.com 
	> Created Time: 2016年09月16日 星期五 10时28分57秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

int main()
{
    //system("ls -l|awk '$5'|sort");
    system("ls -l|sort -n -k 5");
    return 0;
}
