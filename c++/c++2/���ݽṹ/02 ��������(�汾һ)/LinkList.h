#ifndef LINKLIST_H
#define LINKLIST_H

#include<stdlib.h>

//链表节点
typedef struct _LINKNODE{
	void* data; //数据域
	struct _LINKNODE* next; //指针域
}LinkNode;

//链表
typedef struct _LINKLIST{
	LinkNode header; //头结点
	int size;//链表大小
}LList;

typedef void* LinkList;
//比较函数指针
typedef int(DATA_COMPARE)(void*, void*);
//打印函数指针
typedef void(DATA_PRINT)(void*);

//初始化
LinkList Init_LinkList();
//指定位置插入
void Insert_LinkList(LinkList list,int pos,void* data);
//头部插入操作
void PushFront_LinkList(LinkList list, void* data);
//尾部插入操作
void PushBack_LinkList(LinkList list, void* data);
//指定位置删除
void RemoveByPos_LinkList(LinkList list,int pos);
//头部删除操作
void PopFront_LinkList(LinkList list);
//尾部删除操作
void PopBack_LinkList(LinkList list);
//值删除
void RemoveByVal_LinkList(LinkList list, void* data, DATA_COMPARE compare);
//获得指定位置元素
void* Get_LinkList(LinkList list,int pos);
//链表大小
int Size_LinkList(LinkList list);
//遍历链表
void Print_LinkList(LinkList list, DATA_PRINT print);
//销毁链表
void Destroy_LinkList(LinkList list);


#endif