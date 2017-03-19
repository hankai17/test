#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"SeqStack.h"

typedef struct _PERSON{
	char name[64];
	int age;
}Person;

int main(){

	//初始化栈
	SeqStack stack = Init_SeqStack();
	//创建数据
	Person p1 = { "aaa", 10 };
	Person p2 = { "bbb", 20 };
	Person p3 = { "ccc", 30 };
	Person p4 = { "ddd", 40 };
	Person p5 = { "eee", 50 };
	Person p6 = { "fff", 60 };
	Person p7 = { "ggg", 70 };
	//入栈
	Push_SeqStack(stack, (void*)&p1);
	Push_SeqStack(stack, (void*)&p2);
	Push_SeqStack(stack, (void*)&p3);
	Push_SeqStack(stack, (void*)&p4);
	Push_SeqStack(stack, (void*)&p5);
	Push_SeqStack(stack, (void*)&p6);
	Push_SeqStack(stack, (void*)&p7);
	//输出栈中元素
	while (Size_SeqStack(stack) > 0){
		//获得栈顶元素
		Person* person = (Person*)Top_SeqStack(stack);
		printf("Name:%s Age:%d\n",person->name,person->age);
		//出栈
		Pop_SeqStack(stack);
	}
	//销毁栈
	Destroy_SeqStack(stack);

	system("pause");
	return EXIT_SUCCESS;
}