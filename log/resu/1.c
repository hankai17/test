#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<string.h>


static pthread_key_t key; //线程键
pthread_once_t init_done=PTHREAD_ONCE_INIT;

struct student {
    char name[10];
    int age;
};
struct student stu;


void free_fun(void *argv)
{
    printf("free_fun end\n");
}
void print_info(struct student *argv)
{
    printf("name :%s age: %d\n",stu.name,stu.age);
}


void *pthread_fun1(void *argv)
{


    pthread_setspecific(key,(void *)(&stu)); //
    strcpy(stu.name,"name1");
    stu.age=11;
    pthread_t my_tid=pthread_self();
    struct student *s;
    printf("add1=%x\n",s);
    s=pthread_getspecific(key);
    if(s!=NULL){
        print_info(s);
    }
    sleep(5);
    s=pthread_getspecific(key);
    if(s!=NULL){
        print_info(s);
    }
}


void *pthread_fun2(void *agrv)
{
    pthread_setspecific(key,(void *)(&stu)); //
    strcpy(stu.name,"name2");
    stu.age=22;
    pthread_t my_tid=pthread_self();
    struct student *s;
    printf("add2=%x\n",s);
    s=pthread_getspecific(key);
    if(s!=NULL){
        print_info(s);
    }
    sleep(1);
    s=pthread_getspecific(key);
    if(s!=NULL){
        print_info(s);
    }
}


int main()
{
    void *p=NULL;
    pthread_t tid[2];
    pthread_key_create(&key,free_fun);
    pthread_create(&tid[0],NULL,pthread_fun1,NULL);
    pthread_create(&tid[1],NULL,pthread_fun2,NULL);
    int i;
    for(i=0;i<2;i++){
        if(pthread_join(tid[i],&p)==0)
            printf("thread %d is end success\n",i);
    }
    return 0;
}
