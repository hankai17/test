#ifndef LINKLIST_H
#define LINKLIST_H

#include<stdlib.h>

//链表小节点
typedef struct _LINKNODE{
	struct _LINKNODE* next;
}LinkNode;

//链表
typedef struct _LINKLIST{
	LinkNode header;
	int size;
}LList;

typedef void* LinkList;
typedef int(*DATA_COMPARE)(LinkNode*, LinkNode*);
typedef void(*DATA_PRINT)(LinkNode*);

//初始化
LinkList Init_LinkList();
//指定位置插入
void Insert_LinkList(LinkList list, int pos, LinkNode* data);
//头部插入操作
void PushFront_LinkList(LinkList list,LinkNode* data);
//尾部插入操作
void PushBack_LinkList(LinkList list, LinkNode* data);
//指定位置删除
void RemoveByPos_LinkList(LinkList list,int pos);
//头部位置删除
void PopFront_LinkList(LinkList list);
//尾部位置删除
void PopBack_LinkList(LinkList list);
//根据值删除
void RemoveByVal_LinkList(LinkList list, LinkNode* data, DATA_COMPARE compare);
//链表大小
int Size_LinkList(LinkList list);
//打印链表
void Print_LinkList(LinkList list, DATA_PRINT print);
//销毁链表
void Destroy_LinkList(LinkList list);


#endif