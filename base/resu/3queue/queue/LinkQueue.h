#ifndef LINKQUEUE_H
#define LINKQUEUE_H

#include<stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif



	typedef void* LinkQueue;

	//节点定义
	typedef struct _QUEUENODE{
		struct _QUEUENODE* next;
	}QueueNode;


	//链表结构体
	typedef struct _LINKQUEUE{
		QueueNode header; //头结点
		int size; //节点的个数
	}LQueue;


	//初始化
	LinkQueue Init_LinkQueue();
	//入队
	void Push_LinkQueue(LinkQueue queue,void* data);
	//出队
	void Pop_LinkQueue(LinkQueue queue);
	//获得队头元素
	void* Front_LinkQueue(LinkQueue queue);
	//获得队尾元素
	void* Back_LinkQueue(LinkQueue queue);
	//大小
	int Size_LinkQueue(LinkQueue queue);
	//销毁‘
	void Destroy_LinkQueue(LinkQueue queue);


#ifdef __cplusplus
	}
#endif





#endif