#include "LinkList.h"

typedef 
struct _LinkList {
  //struct LinkNode* header;
  //LinkNode* header;
  LinkNode header;
  int size;
} LList;

LinkList Init_LinkList() {
  LList* tmp = (LList *)malloc(sizeof(LList));	
  tmp->size = 0;
  tmp->header.next = NULL;
  return tmp;	
}

void Insert_LinkList(LinkList list,int pos,void *data) {
  LList *tmp = (LList*)list;
  if(pos>tmp->size||pos<0) {
    pos = tmp->size;	
  }
  LinkNode * pCurrent = &(tmp->header);
  int i = 0;
  for(i = 0; i < pos; i++) {
    pCurrent = pCurrent->next;	
  }
  LinkNode* newNode = (LinkNode*)malloc(sizeof(LinkNode));//为什么插入节点时候要malloc一个结构体?
  //因为有 void* 和struct*? 如果不malloc 插入一个节点
  //定义成临时的不妥 所以必须得手动分配内存
  //但是用户传过来的也是分配好的内存空间 但是这个地址必须存放到结构体中
  //结构体链得迎合 用户分配好的内存
  //就是说这个地址与结构体是绑定在一起的 共存亡了 所以得malloc

  //为什么托瓦斯链表没有malloc 
  //因为传过来的是用户定义好的分配好的内存 对其地址进行
  //连接即可
  newNode->data = data;
  //newNode->next=NULL;
  newNode->next = pCurrent->next;
  pCurrent->next = newNode;
  tmp->size++;
}

void RemoveByPos_LinkList(LinkList list,int pos) {
  LList* tmp = (LList*)list;
  int i = 0;
  if(pos > tmp->size || pos < 0) {
    return;
  }
  LinkNode* pCurrent = &(tmp->header);
  for(i = 0; i < pos; i++) {
    pCurrent = pCurrent->next;
  }
  LinkNode* pNode = pCurrent->next;		
  pCurrent->next = pNode->next;
  free(pNode);
  tmp->size--;
}

void *Get_LinkList(LinkList list,int pos) {
  LList *tmp = (LList*)list;
  int i = 0;
  LinkNode* pCurrent = &(tmp->header);
  for(i = 0; i < pos; i++) {
    pCurrent = pCurrent->next;
  }
  return pCurrent->next;
}

int Size_LinkList(LinkList list) {
  LList* tmp = (LList*)list;
  return tmp->size; 
}

void Print_LinkList(LinkList list, DATA_PRINT print) {
  LList*tmp = (LList*)list;
  LinkNode* pCurren = tmp->header.next;
  while (pCurren != NULL) {
    print(pCurren->data);
    pCurren = pCurren->next;
  }
}

//void Print_LinkList(LinkList list,)
void Destroy_LinkList(LinkList list) {
  LList* tmp = (LList*)list;
  LinkNode* pCurrent = (tmp->header.next);
  LinkNode* pNode;
  while(pCurrent != NULL) {
    pNode = pCurrent->next;
    free(pCurrent);
    pCurrent = pNode;
  }
  free(tmp);
}

//bool IsPalindrome(LinkList list) {

void* GetMiddle(LinkList list) {
  LList* tmp = (LList*)list;
  //LinkNode* pCurren = tmp->header;
  if(tmp->header.next->next == NULL) {
    return NULL;
  }

  if((tmp->header.next)->next->next == NULL) {
    RemoveByPos_LinkList(list,0);
    return;
  }

  LinkNode* pPrev = tmp->header.next;
  LinkNode* pCurren = (tmp->header).next->next->next;

  while(pCurren->next != NULL && pCurren->next->next != NULL) {
    pPrev = pPrev->next;
    pCurren = pCurren->next->next;
  }
  pPrev->next = pPrev->next->next;
  return;
}

