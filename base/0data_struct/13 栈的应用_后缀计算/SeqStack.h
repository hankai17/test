#ifndef SEQSTACK_H
#define SEQSTACK_H

#include<stdlib.h>

#define MAXSIZE 1024

typedef struct _SEQSTACK{
	void* data[MAXSIZE];
	int size;
}SStack;

typedef void* SeqStack;

//初始化
SeqStack Init_SeqStack();
//压栈
void Push_SeqStack(SeqStack stack, void* data);
//出栈
void Pop_SeqStack(SeqStack stack);
//获得栈顶元素
void* Top_SeqStack(SeqStack stack);
//大小
int Size_SeqStack(SeqStack stack);
//销毁栈
void Destroy_SeqStack(SeqStack stack);


#endif