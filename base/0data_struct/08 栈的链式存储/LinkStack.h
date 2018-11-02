#ifndef LINKSTACK_H
#define LINKSTACK_H

#include<stdlib.h>

//栈节点
typedef struct _STACKNODE{
	struct _STACKNODE* next;
}StackNode;

//栈
typedef struct _LINKSTACK{
	StackNode header;
	int size;
}LStack;

typedef void* LinkStack;

//初始化
LinkStack Init_LinkStack();
//压栈
void Push_LinkStack(LinkStack stack, StackNode* data);
//出栈
void Pop_LinkStack(LinkStack stack);
//获得栈顶元素
void* Top_LinkStack(LinkStack stack);
//大小
int Size_LinkStack(LinkStack stack);
//销毁栈
void Destroy_LinkStack(LinkStack stack);


#endif