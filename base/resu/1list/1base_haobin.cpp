#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
#include <malloc.h>
#include <stdlib.h>


typedef struct Node
{
	int data;//数据域 
	struct Node *pNext;//指针域 
}NODE,*PNODE;// node等价于 struct node, pnode 等价于struct node *


PNODE create_list(void);
void traverse_list(PNODE pHead); 
bool is_empty(PNODE pHead);
int length_list(PNODE);
bool insert_list(PNODE,int,int);//指向哪个链表 位置 值 
bool delete_list(PNODE,int,int *);//       ..........删除后放哪里
void sort_list(PNODE);
 
PNODE create_list(void)//此函数可动态的分配好内存 ，把头结点的地址给phead，即 phead指向了一个链表了 
{
	int len;//存放有效节点个数 
	int i;
	int val;//临时存放用户输入的节点的值 
	PNODE pHead = (PNODE)malloc(sizeof(NODE));//分配了一个不存放有效数据的头节点 //pHead 是头指针 
	if(NULL==pHead)
	{
		printf("分配失败\n");
		exit(-1);
	}
	PNODE pTail= pHead;//PTail也指向了头结点 phead也指向头结点 一个节点情况下头节点就是为节点 
	pTail->pNext=NULL; 
	printf("输入结点个数=");scanf("%d",&len); 
	for(i=0;i<len;++i)
	{
		printf("输入第%d个节点的值：",i+1); 
		scanf("%d",&val); 
		PNODE pNew = (PNODE)malloc(sizeof(NODE));//每循环一次pnew造出一个新的节点 
	if(NULL==pNew)
	{
		printf("分配失败\n");
		exit(-1);
	}
	pNew->data=val;	//PNEW生成临时的节点，把数据给临时节点的数据域              
    pTail->pNext=pNew;//定义一个指针永远指向最后一个节点 
    pNew->pNext=NULL;
    pTail=pNew;
	}
	return pHead; 
}

void traverse_list(PNODE pHead)//输出的意思 无需返回值 需要参数（在哪个地方的遍历） 
{
	PNODE p = pHead->pNext;//p指向第一个有效节点 
	while(NULL!=p)//指针域不空 
	{
		printf("%d ",p->data);
		p=p->pNext;
	}
	printf("\n");
	return;//函数执行完毕 
} 

bool is_empty(PNODE pHead)
{
	if(NULL==pHead->pNext)//头结点的指针域为空
	 return true;
	 else
	 return false; 
}

int length_list(PNODE pHead)
{
	PNODE p=pHead->pNext;
	int len=0;
	while(NULL!=p)
	{
		++len;
		p=p->pNext;
	}
	return len;
}

void sort_list(PNODE pHead)//很重要//会背 
{	
	int i,j,t;
	PNODE p,q;
	int len=length_list(pHead);
	
	for(i=0,p=pHead->pNext;i<len-1;++i,p=p->pNext)//i是第一个有效元素的下表，p第一个有效元素的地址 
	{
		for(j=i+1,q=p->pNext;j<len;++j,q=q->pNext)
		{
			if(p->data>q->data)
			{
				t=p->data;//a[i]>a[j]
				p->data=q->data;//t=a[i]
				q->data=t;//达到了泛型 a[j]=t
			}
		}
	}
	return; 
}

//在pHead所指向链表的第pos个节点的前面插入一个新的节点，该节点的值是val,并且pos的值从1开始 
bool insert_list(PNODE pHead,int pos,int val)
{//链表不存在 满不满 
	int i=0;
	PNODE p=pHead;
	
	while(NULL!=p&&i<pos-1)
	{
		p=p->pNext;
		++i;
	}
	if(i>pos-1||NULL==p)
	return false;
	
	PNODE pNew=(PNODE)malloc(sizeof(NODE));
	if(NULL==pNew)
	{
		printf("动态分配内存失败\n");
		exit(-1); 
	}
	pNew->data=val;
	PNODE q=p->pNext;
	p->pNext=pNew;
	pNew->pNext=q;
	
	return true;
}

bool delete_list(PNODE pHead,int pos,int * pVal/*in out*/)
{
	int i=0;
	PNODE p=pHead;
	
	while(NULL!=p->pNext&&i<pos-1)
	{
		p=p->pNext;
		++i;
	}
	if(i>pos-1||NULL==p->pNext)
	return false;
	PNODE q=p->pNext;
	*pVal=q->data;
	
	p->pNext=p->pNext->pNext;
	free(q);
	q=NULL;
	system("pause");
	return true;
}
//错误	1	error C4996: 'scanf': This function or variable may be unsafe. Consider using scanf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	i:\c++\笔试模拟\笔试模拟\笔试模拟\1两种基本链表\未命名1.cpp	78	1	笔试模拟

//还没有分配内存 
int main(void)
{
	PNODE pHead = NULL;//等价于 struct node *pHead 
	pHead = create_list();//创建一个非循环单链表 并将该链表的头结点的地址给phead//即实现了 一个函数分配好了内存 将此内存的首地址给phead 
	traverse_list(pHead);//遍历 一个一个输出 
	int val;

	int len = length_list(pHead);
	printf("链表长度是%d\n", len);

	if (is_empty(pHead))
		printf("链表空\n");
	else
		printf("链表不空\n");


	sort_list(pHead);
	traverse_list(pHead);//遍历 一个一个输出 

	insert_list(pHead, 3, 22);
	traverse_list(pHead);//遍历 一个一个输出 

	if (delete_list(pHead, 4, &val))
	{
		printf("删除成功,删除的元素是：%d\n", val);
	}
	else
	{
		printf("删除失败");
	}
	traverse_list(pHead);//遍历 一个一个输出 	
	return 0;
}