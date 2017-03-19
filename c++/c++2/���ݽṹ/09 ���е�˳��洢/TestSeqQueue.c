#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"SeqQueue.h"

typedef struct _PERSON{
	char name[64];
	int age;
}Person;

int main(){


	//初始化队列
	SeqQueue queue = Init_SeqQueue();
	//创建数据
	Person p1 = { "aaa", 10 };
	Person p2 = { "bbb", 20 };
	Person p3 = { "ccc", 30 };
	Person p4 = { "ddd", 40 };
	Person p5 = { "eee", 50 };
	Person p6 = { "fff", 60 };
	//入队
	Push_SeqQueue(queue, (void*)&p1);
	Push_SeqQueue(queue, (void*)&p2);
	Push_SeqQueue(queue, (void*)&p3);
	Push_SeqQueue(queue, (void*)&p4);
	Push_SeqQueue(queue, (void*)&p5);
	Push_SeqQueue(queue, (void*)&p6);
	//打印队头 队尾元素
	Person* front = (Person*)Front_SeqQueue(queue);
	printf("Name:%s Age:%d\n",front->name,front->age);
	Person* back = (Person*)Back_SeqQueue(queue);
	printf("Name:%s Age:%d\n", back->name, back->age);
	printf("----------------------------\n");
	//输出
	while (Size_SeqQueue(queue) > 0){
		//获得队头元素
		front = (Person*)Front_SeqQueue(queue);
		printf("Name:%s Age:%d\n", front->name, front->age);
		//出队操作
		Pop_SeqQueue(queue);
	}
	
	//销毁队列
	Destroy_SeqQueue(queue);

	system("pause");
	return EXIT_SUCCESS;
}