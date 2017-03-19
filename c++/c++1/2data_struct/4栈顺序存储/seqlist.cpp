#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "seqlist.h"

typedef struct _tag_SeqList
{
	int length;
	int capacity;
	unsigned int * node;
}TSeqList;



SeqList * SeqList_Create(int capacity)
{
	TSeqList * tmp = NULL;
	tmp = (TSeqList *)malloc(sizeof(TSeqList));
	//!!ÅÐ¶Ï£¡£¡
	memset(tmp, 0, sizeof(TSeqList));
	tmp->node = (unsigned int *)malloc(sizeof(unsigned int *)*capacity);
	//£¡£¡£¡ÅÐ¶Ï£¡
	tmp->capacity = capacity;
	tmp->length = 0;
    return tmp;
}
//Îö¹¹ËùÓÐ
void SeqList_Destory(SeqList * list)
{
	TSeqList * tlist = NULL;
	//ÅÐ¶Ï
	tlist = (TSeqList *)list;
	if (tlist->node != NULL)
	{
		free(tlist->node);
	}
	free(tlist);
}
//Çå¿ÕÁ´±í »Øµ½³õÊ¼»¯×´Ì¬
void SeqList_Clear(SeqList * list)
{
	TSeqList * tlist = NULL;
	//ÅÐ¶Ï
	tlist = (TSeqList *)list;
	tlist->length = 0;
}

int SeqList_Length(SeqList * list)
{
	TSeqList * tlist = NULL;
	//ÅÐ¶Ï
	tlist = (TSeqList *)list;
	return tlist->length;

}

int SeqList_Capacity(SeqList * list)
{
	TSeqList * tlist = NULL;
	//ÅÐ¶Ï
	tlist = (TSeqList *)list;
	return tlist->capacity;
}

int SeqList_Insert(SeqList * list, SeqListNode * node, int pos)
{
	int i = 0;
	TSeqList * tlist = NULL;
	//ÅÐ¶Ï£¡£¡
	tlist = (TSeqList *)list;

	for (i = tlist->length; i < pos; i--)
	{
		tlist->node[i] = tlist->node[i-1];
	}
	tlist->node[i] = (unsigned int)node;

	return 0;
}

SeqListNode * SeqList_Get(SeqList * list, int pos)
{
	int i = 0;
	SeqListNode * ret = 0;
	TSeqList * tlist = NULL;
	//ÅÐ¶Ï£¡£¡
	tlist = (TSeqList *)list;
	ret = (void *)tlist->node;
	return ret;
}

SeqListNode * SeqList_Delete(SeqList * list, int pos)
{
	int i = 0;
	SeqListNode * ret = 0;
	TSeqList * tlist = NULL;
	//ÅÐ¶Ï£¡£¡£¡
	tlist = (TSeqList*)list;
	ret = (SeqListNode *)tlist->node[pos];

	for (i = pos + 1; i < tlist->length; i++)
	{
		tlist->node[i - 1] = tlist->node[i];
	}
	tlist->length--;
	return ret;

}