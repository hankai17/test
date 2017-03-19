#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "seqlist.h"

typedef struct _tag_SeqList
{
	int length;
	int capacity;
	unsigned int * node;
}TSeqList;//底层结构体里封装了 长度 容量 节点指针

SeqList * SeqList_Create(int capacity)//根据容量创建节点个数，返回底层句柄
{
	TSeqList * tmp = NULL;
	tmp = (TSeqList *)malloc(sizeof(TSeqList));
	if (tmp == NULL)
	{
		printf("creat err!");
	}
	memset(tmp, 0, sizeof(TSeqList));
	tmp->node = (unsigned int *)malloc(sizeof(unsigned int *)*capacity);
	if (tmp->node == NULL)
	{
		printf("creat err!");
	}
	tmp->capacity = capacity;
	tmp->length = 0;
    return tmp;
}
//析构所有
void SeqList_Destory(SeqList * list)
{
	TSeqList * tlist = NULL;
	tlist = (TSeqList *)list;
	if (tlist->node != NULL)
	{
		free(tlist->node);
	}
	free(tlist);
}
//清空链表 回到初始化状态
void SeqList_Clear(SeqList * list)
{
	TSeqList * tlist = NULL;
	tlist = (TSeqList *)list;
	tlist->length = 0;
}

int SeqList_Length(SeqList * list)
{
	TSeqList * tlist = NULL;
	tlist = (TSeqList *)list;
	return tlist->length;

}

int SeqList_Capacity(SeqList * list)
{
	TSeqList * tlist = NULL;
	if (tlist == NULL)
	{
		printf("Capacity err!\n");
	}
	tlist = (TSeqList *)list;
	return tlist->capacity;
}

int SeqList_Insert(SeqList * list, SeqListNode * node, int pos)//
{
	int i = 0;
	int ret = 0;
	TSeqList * tlist = NULL;
	
	if (list == NULL || node == NULL || pos < 0)
	{
		ret = -1;
		printf("insert err!:%d\n", ret);
		return ret;
	}

	tlist = (TSeqList *)list;
	
	if (tlist->length >= tlist->capacity)
	{
		ret = -2;
		printf("insert err!:%d\n", ret);
		return ret;
	}

	if (pos >= tlist->length)
	{
		pos = tlist->length;
	}

	for (i = tlist->length; i>pos; i--)
	{
		tlist->node[i] = tlist->node[i-1];
	}
	tlist->node[i] = (unsigned int)node;
	tlist->length++;
	return 0;

}

int SeqList_Get(SeqList * list, int pos)
{
	int i = 0;
	TSeqList * tlist = NULL;
	tlist = (TSeqList *)list;

	return tlist->node[pos];
}

SeqListNode * SeqList_Delete(SeqList * list, int pos)
{
	int i = 0;
	SeqListNode * ret = 0;
	TSeqList * tlist = NULL;

	tlist = (TSeqList*)list;
	ret = (SeqListNode *)tlist->node[pos];

	for (i = pos + 1; i < tlist->length; i++)
	{
		tlist->node[i - 1] = tlist->node[i];
	}
	tlist->length--;
	return ret;

}