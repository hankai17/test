#ifndef SEQQUEUE_H
#define SEQQUEUE_H

#include<stdlib.h>

#define MAXSIZE 1024

typedef struct _SEQQUEUE{
	void* data[MAXSIZE];
	int size;
}SQueue;

typedef void* SeqQueue;

//初始化
SeqQueue Init_SeqQueue();
//入队
void Push_SeqQueue(SeqQueue queue,void* data);
//出队
void Pop_SeqQueue(SeqQueue queue);
//获得队头元素
void* Front_SeqQueue(SeqQueue queue);
//获得队尾元素
void* Back_SeqQueue(SeqQueue queue);
//获得队列的长度
int Size_SeqQueue(SeqQueue queue);
//销毁队列
void Destroy_SeqQueue(SeqQueue queue);



#endif