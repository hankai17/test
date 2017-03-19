#include <iostream>
#include <string.h>
#include <stdio.h>
#include "linklist.h"

typedef struct _tag_LinkList
{
	LinkListNode header;
	int length;
}TLinkList;


LinkList * LinkList_Create()
{
	TLinkList * ret = NULL;
	ret = (TLinkList *)malloc(sizeof(TLinkList));
	memset(ret, 0, sizeof(TLinkList));
	ret->length = 0;
	ret->header.next = NULL;

	return ret;
}

void LinkList_Destroy(LinkList * list)
{
	if (list != NULL)
	{
		free(list);
		list = NULL;
	}

	return;
}

void LinkList_Clear(LinkList * list)
{
	TLinkList * tList = NULL;
	if (list == NULL)
	{
		return;
	}
	tList = (TLinkList *)list;

	tList->length = 0;
	tList->header.next = NULL;
	return;
}

int LinkList_Length(LinkList * list)
{
	TLinkList * tList = NULL;
	if (list == NULL)
	{
		return 0;
	}
	tList = (TLinkList *)list;

	return tList->length;
}

int LinkList_Insert(LinkList * list, LinkListNode * node, int pos)
{
	int i = 0;
	int ret = 0;
	TLinkList * tList = NULL;
	LinkListNode * current = NULL;
	if (list == NULL || node == NULL || pos < 0)
	{
		ret = 0;
		printf("err\n");
		return ret;
	}
	tList = (TLinkList *)list;
	current = &(tList->header);

	for (i = 0; i < pos; i++)
	{
		current = current->next;

	}
	node->next = current->next;
	current->next = node;
	tList->length++;

	return 0;
}

LinkListNode * LinkList_Get(LinkList * list, int pos)
{
	int i = 0;
	int ret = 0; 
	TLinkList * tList = NULL;
	LinkListNode * current = NULL;
	if (list == NULL || pos < 0)
	{
		ret = 0;
		printf("err\n");
		return NULL;
	}
	tList = (TLinkList *)list;
	current = &(tList->header);

	for (i = 0; i < pos; i++)
	{
		current = current->next;

	}
	return current->next;
}

LinkListNode * LinkList_Delete(LinkList * list, int pos)
{
	int i = 0;
	LinkListNode * ret = NULL;
	LinkListNode * current = NULL;
	TLinkList * tList = NULL;
	if (list == NULL || pos < 0)
	{
		ret = 0;
		printf("err\n");
		return NULL;
	}
	tList = (TLinkList *)list;
	current = &(tList->header);

	for (i = 0; i < pos; i++)
	{
		current = current->next;

	}
	ret = current->next; 
	current->next = ret->next;
	tList->length--;
	return ret;
}