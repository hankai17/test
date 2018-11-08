/*************************************************************************
	> File Name: test.c
	> Author:red devil Manchester 
	> Mail:867614535@qq.com 
	> Created Time: 2016年10月08日 星期六 22时51分19秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
pthread_mutex_t mutex;

int main()
{
    int a=100;
    int ret;
    pthread_mutex_init(&mutex, NULL);

    ret=pthread_mutex_unlock(&mutex);
    printf("ret:%d\n",ret);

    ret=pthread_mutex_lock(&mutex);
    printf("ret:%d\n",ret);

    pthread_mutex_destroy(&mutex);
    
}
