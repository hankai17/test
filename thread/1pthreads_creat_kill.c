#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
void *tfn(void *arg)
{
	int i;

	i = (int)arg;
    while(1)
    {
	    printf("I'm %dth thread, Thread_ID = %lu\n", i+1, pthread_self());
        sleep(3);
    }
	return NULL;
}

int thread_kill(pthread_t tid)
{
    //int ret = pthread_kill(tid, 0);
    int ret = pthread_kill(tid, 9);
    if(ret == ESRCH)
    {
        printf("线程不存在或已经终止\n");
    }
    else if( ret == EINVAL)
    {
        printf("信号不合法\n");
    }
    else if(ret == 0)
    {
        printf("kill success\n");
    }
    return 1;
}

int thread_canl(pthread_t tid)
{
    int ret = pthread_cancle(tid);
    if()
}

int main(int argc, char *argv[])
{
	int n = 5, i;
	pthread_t tid;
    pthread_t pthread_array[5];

	for (i = 0; i < n; i++) 
    {
		pthread_create(&tid, NULL, tfn, (void *)i);
        pthread_array[i] = tid;
		//将i转换为指针，在tfn中再强转回整形。
	}

	//sleep(n);
	//printf("I am main, and I am not a process, I'm a thread!\n" "main_thread_ID = %lu\n", pthread_self());

    thread_kill(pthread_array[0]);
    thread_kill(pthread_array[1]);

    //pthread_join(pthread_array[0],0);
    //pthread_join(pthread_array[1],0);
    while(1);
	return 0;
}
