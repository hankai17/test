#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "seqstack.h"
#include "seqlist.h"
//创建栈 相当于 创建线性表

SeqStack * SeqStack_Create(int capacity)
{
	return SeqList_Create(capacity);
	
}

void SeqStack_Destory(SeqStack * stack)
{
	 SeqList_Destory(stack);
}
void SeqStack_Clear(SeqStack * stack)
{
	 SeqList_Clear(stack);
}
//相当于向  的尾部插入元素
int SeqStack_Push(SeqStack * stack, void * item)
{
	return SeqList_Insert(stack, item, SeqList_Length(stack));//尾插法
	
}
//相当从尾部拿出元素
void * SeqStack_Pop(SeqStack * stack)
{

	return SeqList_Delete(stack, SeqList_Length(stack) - 1);
	
}
//相当获取链表尾部元素
void * SeqStack_Top(SeqStack * stack)
{
	return SeqList_Get(stack, SeqList_Length(stack) - 1);
}

int SeqStack_Size(SeqStack * stack)
{
	return SeqList_Length(stack);
}

int SeqStack_Capacity(SeqStack * stack)
{
	return SeqList_Capacity(stack);
}