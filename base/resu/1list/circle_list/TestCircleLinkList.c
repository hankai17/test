#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"CircleLinkList.h"
typedef struct _PERSON{
	CircleLinkNode node;
	char name[64];
	int age;
}Person;


void MyPrint(CircleLinkNode* data){
	Person* p = (Person*)data;
	printf("Name:%s Age:%d\n", p->name, p->age);
}

int MyCompare(CircleLinkNode* data1, CircleLinkNode* data2){
	Person* p1 = (Person*)data1;
	Person* p2 = (Person*)data2;
	return strcmp(p1->name, p2->name) == 0 && p1->age == p2->age;
}

int main(){

	//初始化链表
	CircleLinkList list = Init_CircleLinkList();
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

	//头插
	PushFront_CircleLinkList(list, (CircleLinkNode*)&p1);
	PushFront_CircleLinkList(list, (CircleLinkNode*)&p2);
	//尾插
	PushBack_CircleLinkList(list, (CircleLinkNode*)&p3);
	PushBack_CircleLinkList(list, (CircleLinkNode*)&p4);
	//指定位置插入
	Insert_CircleLinkList(list, 1, (CircleLinkNode*)&p5);
	Insert_CircleLinkList(list, 1, (CircleLinkNode*)&p6);
	//打印
	Print_CircleLinkList(list, MyPrint);
	//头删
	PopFront_CircleLinkList(list);
	printf("头删结果:\n");
	Print_CircleLinkList(list, MyPrint);
	printf("---------------------\n");
	//尾删
	PopBack_CircleLinkList(list);
	printf("尾删结果:\n");
	Print_CircleLinkList(list, MyPrint);
	printf("---------------------\n");
	//指定位置删除
	RemoveByPos_CircleLinkList(list, 1);
	printf("指定位置1删结果:\n");
	Print_CircleLinkList(list, MyPrint);
	printf("---------------------\n");
	printf("链表长度:%d\n", Size_CircleLinkList(list));
	//根据值删除
	RemoveByVal_CircleLinkList(list, (CircleLinkNode*)&p3, MyCompare);
	printf("根据值删结果:\n");
	Print_CircleLinkList(list, MyPrint);
	printf("---------------------\n");
	//销毁链表
	Destroy_CircleLinkList(list);


	system("pause");
	return EXIT_SUCCESS;
}