/*************************************************************************
	> File Name: libevent.c
	> Author:red devil Manchester 
	> Mail:867614535@qq.com 
	> Created Time: 2016年11月10日 星期四 15时06分02秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<event2/event.h>

int main()
{
    struct event_base *base=NULL;
    //创建句柄
    base=event_base_new();

    //添加事件
    
    
    //监听事件状态
    event_base_dispatch(base);
    

    event_base_free(base);

    return 0;
}


