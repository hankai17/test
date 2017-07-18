#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_key_t   key;


/*
 * @brief   free private struct
 * @param[in]  arg    pointer of private struct    
 */
void destr(void *arg)
{
    int *p = (int *)arg;

    printf("*arg = %u\n", (unsigned int)*p);
    free(p);
}


void test(void)
{
    int *p = (int *)pthread_getspecific(key);

    printf("*p = %u\n", (unsigned int)*p);
}

/*
 * @brief   thread proc
 * @param[in]  arg    thread parameter
 * @return      
 */

void *thread(void *arg)
{
    int i;
    pthread_t id = pthread_self();
    int *p = (int *)malloc(sizeof(int));

    printf("id = %u\n", (int)id);
    *p = (int)id;
    pthread_setspecific(key, p);
    test();

    return (void *)0;
}

int main(void)
{
    pthread_t id;
    int i;
    int ret;
    int pthread_arg = 100;
    void *pthread_ret;

    pthread_key_create(&key, destr);

    ret = pthread_create(&id, NULL, thread, &pthread_arg);
    if (ret != 0)
    {
        printf ("Create pthread error!\n");
        exit (1);
    }

    ret = pthread_create(&id, NULL, thread, &pthread_arg);
    if (ret != 0)
    {
        printf ("Create pthread error!\n");
        exit (1);
    }

    sleep(5);
    pthread_join(id, &pthread_ret);
    printf("pthread_ret = %d\n", (int)pthread_ret);

    return (0);
}
