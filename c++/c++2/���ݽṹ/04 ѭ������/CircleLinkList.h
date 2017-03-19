#ifndef CIRCLELINKLIST_H
#define CIRCLELINKLIST_H

#include<stdlib.h>

//链表节点
typedef struct _CIRCLELINKNODE{
	struct _CIRCLELINKNODE* next;
}CircleLinkNode;

//链表
typedef struct _CIRCLELINKLIST{
	CircleLinkNode header;
	int size;
}CLinkList;

typedef void* CircleLinkList;
//比较函数指针
typedef int(*DATA_COMPARE)(CircleLinkNode*, CircleLinkNode*);
//打印函数指针
typedef void(*DATA_PRINT)(CircleLinkNode*);

//初始化
CircleLinkList Init_CircleLinkList();
//指定位置插入
void Insert_CircleLinkList(CircleLinkList clist, int pos, CircleLinkNode* data);
//头部插入操作
void PushFront_CircleLinkList(CircleLinkList clist, CircleLinkNode* data);
//尾部插入操作
void PushBack_CircleLinkList(CircleLinkList clist, CircleLinkNode* data);
//指定位置删除
void RemoveByPos_CircleLinkList(CircleLinkList clist,int pos);
//头部删除操作
void PopFront_CircleLinkList(CircleLinkList clist);
//尾部删除操作
void PopBack_CircleLinkList(CircleLinkList clist);
//值删除
void RemoveByVal_CircleLinkList(CircleLinkList clist, CircleLinkNode* data, DATA_COMPARE compare);
//获得指定位置元素
void* Get_CircleLinkList(CircleLinkList clist,int pos);
//链表大小
int Size_CircleLinkList(CircleLinkList clist);
//打印链表
void Print_CircleLinkList(CircleLinkList clist, DATA_PRINT print);
//销毁链表
void Destroy_CircleLinkList(CircleLinkList clist);

#endif