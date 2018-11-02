#ifndef _LINKLIST_H
#define _LINKLIST_H
#include<stdlib.h>
#include<stdio.h>

typedef 
struct _LINKNODE {
  void *data;
  struct _LINKNODE*next;
} LinkNode; 

typedef void* LinkList;
typedef void(DATA_PRINT)(void*);

LinkList Init_LinkList();

void Insert_LinkList(LinkList list,int pos,void *data);

void RemoveByPos_LinkList(LinkList list,int pos);

void* Get_LinkList(LinkList list,int pos);

int Size_LinkList(LinkList list);

//void Print_LinkList(LinkList list,)
void Print_LinkList(LinkList list, DATA_PRINT print);

void Destroy_LinkList(LinkList list);

bool IsPalindrome(LinkList list);

void* GetMiddle(LinkList list);

#endif
