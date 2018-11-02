#ifndef LINKSTACK_H
#define LINKSTACK_H

#include<stdlib.h>

#ifdef __cplusplus
extern "C"{
#endif

	//栈的节点
	typedef struct _STACKNODE{
		struct _STACKNODE* next;
	}StackNode;


	//栈结构体
	typedef struct _LINKSTACK{
		StackNode* head; //指向链表的第一个节点
		int size;
	}LStack;

	typedef void* LinkStack;

	//初始化
	LinkStack Init_LinkStack();
	//压栈
	void Push_LinkStack(LinkStack stack, void* data);
	//出栈
	void Pop_LinkStack(LinkStack stack);
	//获得栈顶元素
	void* Top_LinkStack(LinkStack stack);
	//大小
	int Size_LinkStack(LinkStack stack);
	//销毁
	void Destroy_LinkStack(LinkStack stack);

#ifdef __cplusplus
	}
#endif

#endif