#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex;
int a=99;

void *tfn(void *arg)
{
	int i;
    int ret1,ret2;

	i = (int)arg;
	//sleep(i);	 //通过i来区别每个线程
	printf("I'm %dth thread, Thread_ID = %lu\n", i+1, pthread_self());
    
    if(i==0)
    {
        printf("begin lock\n");
        //pthread_mutex_lock(&mutex);
        sleep(1);
        ret1=pthread_mutex_lock(&mutex);
        if(ret1!=0)
        {
            printf("err\n");
            printf("lock err:%s",strerror(ret1));
        }
        a=1;
        printf("lock ok\n");
    }

    if(i==1)
    {
    pthread_mutex_destroy(&mutex);
        printf("begin unlock\n");
        ret2=pthread_mutex_unlock(&mutex);
        if(ret2!=0)
        {
            printf("err\n");
            printf("unlock err:%s",strerror(ret1));
        }
        printf("unlock ok\n");
    }

	return NULL;
}

int main(int argc, char *argv[])
{
	int n = 2, i;
	pthread_t tid;
    pthread_mutex_init(&mutex,NULL);

	for (i = 0; i < n; i++) 
    {
		pthread_create(&tid, NULL, tfn, (void *)i);
	}
    while(1);
	//sleep(n);
	//printf("I am main, and I am not a process, I'm a thread!\n" 
			//"main_thread_ID = %lu\n", pthread_self());

	return 0;
}
