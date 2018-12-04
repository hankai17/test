#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"DoubleLinkList.h"

typedef struct _PERSON{
	DoubleLinkNode node;
	char name[64];
	int age;
}Person;

void MyPrint(DoubleLinkNode* data){
	Person* p = (Person*)data;
	printf("Name:%s Age:%d\n",p->name,p->age);
}

int main(){

	//创建双向链表
	DoubleLinkList dlist = Init_DoubleLinkList();
	//创建数据
	Person p1, p2, p3, p4, p5, p6;
	strcpy(p1.name, "aaa");
	strcpy(p2.name, "bbb");
	strcpy(p3.name, "ccc");
	strcpy(p4.name, "ddd");
	strcpy(p5.name, "eee");
	strcpy(p6.name, "fff");
	p1.age = 10;
	p2.age = 20;
	p3.age = 30;
	p4.age = 40;
	p5.age = 50;
	p6.age = 60;
	
	//尾插
	PushBack_DoubleLinkList(dlist, (DoubleLinkNode*)&p1);
	PushBack_DoubleLinkList(dlist, (DoubleLinkNode*)&p2);
	//头插
	PushFront_DoubleLinkList(dlist, (DoubleLinkNode*)&p3);
	PushFront_DoubleLinkList(dlist, (DoubleLinkNode*)&p4);
	//指定位置插入
	Insert_DoubleLinkList(dlist, 1, (DoubleLinkNode*)&p5);
	Insert_DoubleLinkList(dlist, 1, (DoubleLinkNode*)&p6);
	//打印
	Print_DoubleLinkList(dlist, MyPrint);
	printf("逆序打印...\n");
	RPrint_DoubleLinkList(dlist,MyPrint);
	//头删
	PopFront_DoubleLinkList(dlist);
	printf("头删后结果:\n");
	Print_DoubleLinkList(dlist, MyPrint);
	printf("----------------\n");
	//尾删
	PopBack_DoubleLinkList(dlist);
	printf("尾删后结果:\n");
	Print_DoubleLinkList(dlist, MyPrint);
	printf("----------------\n");
	//指定位置删除
	RemoveByPos_DoubleLinkList(dlist,3);
	printf("指定位置删后结果:\n");
	Print_DoubleLinkList(dlist, MyPrint);
	printf("----------------\n");
	printf("链表大小:%d\n",Size_DoubleLinkList(dlist));
	//销毁链表
	Destroy_DoubleLinkList(dlist);

	system("pause");
	return EXIT_SUCCESS;
}