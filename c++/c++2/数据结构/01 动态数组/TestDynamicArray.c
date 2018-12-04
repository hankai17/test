#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"DynamicArray.h"

typedef struct _PERSON{
	char name[64];
	int age;
}Person;

//打印函数
void MyPrint(void* data){
	Person* p = (Person*)data;
	printf("Name:%s Age:%d\n",p->name,p->age);
}

//比较函数
int MyCompare(void* data1,void* data2){
	Person* p1 = (Person*)data1;
	Person* p2 = (Person*)data2;
	return strcmp(p1->name, p2->name) == 0 && p1->age == p2->age;
}

//测试动态数组
void test01(){

	//初始化数组
	DynamicArray darray = Init_DynamicArray();
	Person p1 = { "aaa", 12 };
	Person p2 = { "bbb", 13 };
	Person p3 = { "ccc", 14 };
	Person p4 = { "ddd", 15 };
	Person p5 = { "eee", 16 };
	Person p6 = { "fff", 17 };

	//尾插
	PushBack_DynamicArray(darray, &p1);
	PushBack_DynamicArray(darray, &p2);
	//头插
	PushFront_DynamicArray(darray, &p3);
	PushFront_DynamicArray(darray, &p4);
	//指定位置插入
	Insert_DynamicArray(darray, 3, &p5);
	Insert_DynamicArray(darray, 3, &p6);
	//打印
	Print_DynamicArray(darray, MyPrint);
	//获得指定位置元素
	Person* person = (Person*)Get_DynamicArray(darray, 3);
	printf("指定位置元素: Name:%s Age:%d\n",person->name,person->age);
	//打印容量和大小
	printf("大小:%d\n", Size_DynamicArray(darray));
	printf("容量:%d\n", Capacity_DynamicArray(darray));
	//头删
	PopFront_DynamicArray(darray);
	printf("头删后结果:\n");
	Print_DynamicArray(darray, MyPrint);
	printf("-----------------\n");
	//尾删
	PopBack_DynamicArray(darray);
	printf("尾删后结果:\n");
	Print_DynamicArray(darray, MyPrint);
	printf("-----------------\n");
	//指定位置删
	RemoveByPos_DynamicArray(darray,2);
	printf("删除2位置结果:\n");
	Print_DynamicArray(darray, MyPrint);
	printf("-----------------\n");
	//根据值删
	RemoveByVal_DynamicArray(darray, (void*)&p1, MyCompare);
	printf("根据值删除结果:\n");
	Print_DynamicArray(darray, MyPrint);
	printf("-----------------\n");
	//销毁数组
	Destroy_DynamicArray(darray);
	darray = NULL;
	

}

int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}