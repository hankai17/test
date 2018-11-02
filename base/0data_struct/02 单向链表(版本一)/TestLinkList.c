#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LinkList.h"

typedef struct _PERSON{
	char name[64];
	int age;
}Person;


void MyPrint(void* data){
	Person* p = (Person*)data;
	printf("Name:%s Age:%d\n",p->name,p->age);
}

int MyCompare(void* data1,void* data2){
	Person* p1 = (Person*)data1;
	Person* p2 = (Person*)data2;
	return strcmp(p1->name, p2->name) == 0 && p1->age == p2->age;
}

int main(){

	//初始化链表
	LinkList list = Init_LinkList();
	//创建数据
	Person p1 = { "aaa", 12 };
	Person p2 = { "bbb", 13 };
	Person p3 = { "ccc", 14 };
	Person p4 = { "ddd", 15 };
	Person p5 = { "eee", 16 };
	Person p6 = { "fff", 17 };
	//头插
	PushFront_LinkList(list, (void*)&p1);
	PushFront_LinkList(list, (void*)&p2);
	//尾插
	PushBack_LinkList(list, (void*)&p3);
	PushBack_LinkList(list, (void*)&p4);
	//指定位置插入
	Insert_LinkList(list, 2, (void*)&p5);
	//Insert_LinkList(list, 1, (void*)&p6);
	//打印
	Print_LinkList(list, MyPrint);
	//头删
	PopFront_LinkList(list);
	printf("头删结果:\n");
	Print_LinkList(list, MyPrint);
	printf("---------------------\n");
	//尾删
	PopBack_LinkList(list);
	printf("尾删结果:\n");
	Print_LinkList(list, MyPrint);
	printf("---------------------\n");
	//指定位置删除
	RemoveByPos_LinkList(list,1);
	printf("指定位置1删结果:\n");
	Print_LinkList(list, MyPrint);
	printf("---------------------\n");
	printf("链表长度:%d\n",Size_LinkList(list));
	//根据值删除
	RemoveByVal_LinkList(list, (void*)&p1, MyCompare);
	printf("根据值删结果:\n");
	Print_LinkList(list, MyPrint);
	printf("---------------------\n");
	//销毁链表
	Destroy_LinkList(list);

	system("pause");
	return EXIT_SUCCESS;
}