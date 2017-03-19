#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "linkstack.h"
#include "linklist.h"
/*
typedef struct Teacher//链表的业务节点
{
	LinkListNode node;
	void * item;//栈的业务节点
}Teacher;
*/
typedef struct _tag_LinkStackNode//链表的业务节点
{
	LinkListNode node;
	void * item;//栈的业务节点
}TLinkStackNode;


//创建栈 相当于 创建一个线性表
LinkStack * LinkStack_Create()
{

	return LinkStack_Create();
}

void LinkStack_Destory(LinkStack * stack)
{
	LinkList_Clear(stack);
	LinkList_Destroy(stack);
	
}
//清空栈 涉及到栈元素生命周期管理
void LinkStack_Clear(LinkStack * stack)
{
	if (stack == NULL)
	{
		return;
	}
	while (LinkList_Length(stack)>0)
	{
		LinkStack_Pop(stack);//在这个函数中释放节点内存

	}


	return;
}
//向栈中添加元素 相当 向线性表的头部插入元素
//void * item 栈的业务节点----》转化成 链表的业务节点
int LinkStack_Push(LinkStack * stack, void * item)
{
	TLinkStackNode * tmp = NULL;
	int ret = 0;
	tmp = (TLinkStackNode *)malloc(sizeof(TLinkStackNode));
	if (tmp == NULL)
	{
		return -1;
	}
	memset(tmp, 0, sizeof(TLinkStackNode));
	tmp->item = item;

	ret=LinkList_Insert(stack,(LinkListNode *)tmp,0);
	if (ret != 0)
	{
		printf("err\n");
		if (tmp != NULL)
		{
			free(tmp);
		}
	}
	return ret;
}
//从线性表的头部删除元素
void * LinkStack_Pop(LinkStack * stack)
{
	void * item = NULL;//栈的业务节点
	TLinkStackNode *tmp = NULL;
	tmp=(TLinkStackNode *)LinkList_Delete(stack, 0);
	if (tmp == NULL)
	{
		return NULL;
	}
	item=tmp->item;
	//因为insert时候分配了内存 所以要释放内存
	free(tmp);
	return item;
}

void * LinkStack_Top(LinkStack * stack)
{
	TLinkStackNode * tmp = NULL;
	tmp=(TLinkStackNode *)LinkList_Get(stack, 0);
	if (tmp == NULL)
	{
		return NULL;
	}
	return tmp->item;
}

int LinkStack_Size(LinkStack * stack)
{
	
	return LinkList_Length(stack);
}