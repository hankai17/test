#include <iostream>
#include <pthread.h>

void* ThreadFun(void* arg)
{
    int *value=static_cast<int*> (arg);
    std::cout<<"This is thread"<<*value<<std::endl;
    pthread_exit(0);
}

int main()
{
    int  ret=0;
    pthread_t thread_id1,thread_id2;

    int* v1=new int(1);
    int* v2=new int(2);
    ret = pthread_create(&thread_id1, NULL, ThreadFun, static_cast<void*>(v1)); 
    if (ret)
    {
        std::cout<<"Create pthread error!"<<std::endl;
        return 1;
    }

    ret = pthread_create(&thread_id2, NULL, ThreadFun, static_cast<void*>(v2));
    if (ret)
    {
        std::cout<<"Create pthread error!"<<std::endl;
        return 1;
    }

    pthread_join(thread_id1, NULL);
    pthread_join(thread_id2, NULL);

    return 0;
}