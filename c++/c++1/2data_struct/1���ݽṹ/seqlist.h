#ifndef _MY_SEQLIST_H_
#define _MY_SEQLIST_H_

typedef void SeqList;
typedef void SeqListNode;

SeqList * SeqList_Create(int capacity);

void SeqList_Destory(SeqList * list);

void SeqList_Clear(SeqList * list);

int SeqList_Length(SeqList * list);

int SeqList_Capacity(SeqList * list);

int SeqList_Insert(SeqList * list,SeqListNode * node,int pos);

int SeqList_Get(SeqList * list, int pos);

SeqListNode * SeqList_Delete(SeqList * list, int pos);

#endif

