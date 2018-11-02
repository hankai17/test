#ifndef LINKQUEUE_H
#define LINKQUEUE_H

#include<stdlib.h>

//链式队列节点
typedef struct _QUEUENODE{
	struct _QUEUENODE* next;
}QueueNode;


//队列
typedef struct _LINKQUEUE{
	QueueNode header;
	int size;
}LQueue;

typedef void* LinkQueue;

//初始化
LinkQueue Init_LinkQueue();
//入队
void Push_LinkQueue(LinkQueue queue, QueueNode* data);
//出队
void Pop_LinkQueue(LinkQueue queue);
//获得队头元素
void* Front_LinkQueue(LinkQueue queue);
//获得队尾元素
void* Back_LinkQueue(LinkQueue queue);
//队列大小
int Size_LinkQueue(LinkQueue queue);
//销毁队列
void Destroy_LinkQueue(LinkQueue queue);


#endif