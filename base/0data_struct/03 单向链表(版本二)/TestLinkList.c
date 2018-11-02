#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"LinkList.h"


typedef struct _PERSON{
	LinkNode node;
	char name[64];
	int age;
}Person;

void MyPrint(LinkNode* data){
	Person* person = (Person*)data;
	printf("Name:%s Age:%d\n",person->name,person->age);
}

int MyCompare(LinkNode* data1,LinkNode* data2){
	Person* person1 = (Person*)data1;
	Person* person2 = (Person*)data2;
	return strcmp(person1->name, person2->name) == 0 && person1->age == person2->age;
}

int main(){

	//初始化
	LinkList list = Init_LinkList();
	//创建数据
	Person p1,p2,p3,p4,p5,p6;
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
	PushFront_LinkList(list, (LinkNode*)&p1);
	PushFront_LinkList(list, (LinkNode*)&p2);
	//尾插
	PushBack_LinkList(list, (LinkNode*)&p3);
	PushBack_LinkList(list, (LinkNode*)&p4);
	//指定位置插入
	Insert_LinkList(list, 2, (LinkNode*)&p5); 
	Insert_LinkList(list, 3, (LinkNode*)&p6);
	//Insert_LinkList(list, 3, (LinkNode*)&p6);

	//打印b a e f c d
	Print_LinkList(list, MyPrint);
	//尾删
	PopBack_LinkList(list);
	printf("尾删结果:\n");
	Print_LinkList(list, MyPrint);
	printf("----------------\n");
	//头删
	PopFront_LinkList(list);
	printf("头删结果:\n");
	Print_LinkList(list, MyPrint);
	printf("----------------\n");
	//指定位置删除
	RemoveByPos_LinkList(list,2);
	printf("指定位置2删除结果:\n");
	Print_LinkList(list, MyPrint);
	printf("----------------\n");
	//值删除
	RemoveByVal_LinkList(list, (LinkNode*)&p3, MyCompare);
	printf("删除ccc删除结果:\n");
	Print_LinkList(list, MyPrint);
	printf("----------------\n");


	//销毁链表
	Destroy_LinkList(list);

	system("pause");
	return EXIT_SUCCESS;
}