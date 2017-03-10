#include "queue.h"

QUEUE_CONTROL* g_queue_ctl; 
pthread_t g_pid[15] = {-1};
pthread_mutex_t g_shutdown_mutex; 

void* thread_call_back(void* pvdata)
{
    QUEUE_ENTRY* queue_entry = NULL;
    QUEUE_CONTROL* para = (QUEUE_CONTROL*)pvdata;
    int ret; 
    while(1)
    {
       queue_entry = queue_get_first(para->pstQueuControl);

    }
}

int main()
{
    g_queue_ctl = new_queue();
    for(int i=0; i<15; i++) 
    {
        pthread_create(&g_pid[i],NULL,thread_call_back,(void*)(g_queue_ctl));
    }

    return 0;
}

