/*************************************************************************
	> File Name: 0thread.cpp
	> Author: guaz
	> Mail: hankai17@126.com 
	> Created Time: 2017-02-08 09:29:26
 ************************************************************************/

#include<iostream>  
#include<pthread.h>  
#include<unistd.h>  
using namespace std;  

//int var=5;
__thread int var=5;
//static __thread int var=5;
void* worker(void* arg)
{
	cout<<++var<<endl;
	//cout<<std::this_thread::get_id()<<endl;
	cout<<(unsigned int)pthread_self()<<endl;
	printf("%u \n",pthread_self());
}
int main()
{
	pthread_t pid1,pid2;
	pthread_create(&pid1,NULL,worker,NULL);
	pthread_create(&pid1,NULL,worker,NULL);
	pthread_join(pid1,NULL);
	pthread_join(pid1,NULL);
	cout<<"main var = "<<var<<endl;
	return 0;
}





/*
#include<iostream>  
#include<pthread.h>  
#include<unistd.h>  
using namespace std;  
 
__thread int var=5;
 
void* worker1(void* arg);  
void* worker2(void* arg);  
int main(){  
    pthread_t pid1,pid2;  
  
    static __thread  int temp=10;//���κ����ڵ�static����  
    pthread_create(&pid1,NULL,worker1,NULL);  
    pthread_create(&pid2,NULL,worker2,NULL);  
    pthread_join(pid1,NULL);  
    pthread_join(pid2,NULL);  
    cout<<temp<<endl;//���10  
    return 0;  
}  
void* worker1(void* arg){  
    cout<<++var<<endl;//��� 6  
}  
void* worker2(void* arg){  
    sleep(1);//�ȴ��߳�1�ı�varֵ����֤�Ƿ�Ӱ���߳�2  
    cout<<++var<<endl;//���6  
} 
*/ 
