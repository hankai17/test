/*************************************************************************
  > File Name: 1.cpp
  > Author: guaz
  > Mail: hankai17@126.com 
  > Created Time: 2017-03-01 16:23:40
 ************************************************************************/

#include<stdio.h>
#include<sys/time.h>
#include<unistd.h>

int main()
{
    struct  timeval    tv;
    struct  timezone   tz;
    gettimeofday(&tv,&tz); //当前时间,时区
    
    printf("tv_sec:%d\n",tv.tv_sec);
    printf("tv_usec:%d\n",tv.tv_usec);

    printf("tz_minuteswest:%d\n",tz.tz_minuteswest);
    printf("tz_dsttime:%d\n",tz.tz_dsttime);
    return 0;
}
