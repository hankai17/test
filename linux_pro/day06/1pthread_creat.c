#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *tfn(void *arg)
{
	printf("I'm thread, Thread_ID = %lu\n", pthread_self());
	return NULL;
}

int main(void)
{
	pthread_t tid;

	pthread_create(&tid, NULL, tfn, NULL);
	sleep(1);
	printf("I am main, my pid = %d\n", getpid());

	return 0;
}
