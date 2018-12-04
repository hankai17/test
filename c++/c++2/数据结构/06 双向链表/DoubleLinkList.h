#ifndef DOUBLELINKLIST_H
#define DOUBLELINKLIST_H

#include<stdlib.h>

//链表节点
typedef struct _DOUBLELINKNODE{
	struct _DOUBLELINKNODE* prev;
	struct _DOUBLELINKNODE* next;
}DoubleLinkNode;

//链表
typedef struct _DOUBLELINKLIST{
	DoubleLinkNode header;
	int size;
}DLinkList;

typedef void* DoubleLinkList;
//打印
typedef void(*DATA_PRINT)(DoubleLinkNode*);

//初始化
DoubleLinkList Init_DoubleLinkList();
//指定位置插入
void Insert_DoubleLinkList(DoubleLinkList dlist, int pos, DoubleLinkNode* data);
//头部插入操作
void PushFront_DoubleLinkList(DoubleLinkList dlist, DoubleLinkNode* data);
//尾部插入操作
void PushBack_DoubleLinkList(DoubleLinkList dlist, DoubleLinkNode* data);
//指定位置删除
void RemoveByPos_DoubleLinkList(DoubleLinkList dlist,int pos);
//头部删除操作
void PopFront_DoubleLinkList(DoubleLinkList dlist);
//尾部删除操作
void PopBack_DoubleLinkList(DoubleLinkList dlist);
//链表大小
int Size_DoubleLinkList(DoubleLinkList dlist);
//正向打印链表
void Print_DoubleLinkList(DoubleLinkList dlist, DATA_PRINT print);
//反向打印链表
void RPrint_DoubleLinkList(DoubleLinkList dlist, DATA_PRINT print);
//销毁链表
void Destroy_DoubleLinkList(DoubleLinkList dlist);


#endif