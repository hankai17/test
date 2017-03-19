#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//定义一个数组类型

//定义一个指针数组类型

//定义一个指向 数组类的指针 数组类的指针

void main1()
{
	int a[10];//a代表数组首元素地址 &a是整个数组的地址 a+1步长4  &a+1步长40
	/*{
	//定义一个数组类型
	typedef int(myTypeArray)[10];
	myTypeArray myArray;
	myArray[0] = 10;
	printf("%d\n", myArray[0]);
	}*/
	/*
	//定义个指针数组类型
	typedef int(*PTypeArray)[10];
	PTypeArray myParray;
	myParray = &a;

	*myParray[0] = 30;
	printf("%d\n", a[0]);

	*/
	{
		//定义一个指向 数组的 指针
		int(*MyPointer)[10];//变量 告诉编译器分配内存//相当于 typedef int(*PTypeArray)[10];PTypeArray myParray;
		MyPointer = &a;
		*MyPointer[0] = 40;
		printf("%d\n", a[0]);

	}
	system("pause");
	return;
}

//函数指针语法梳理
//1 如何定义一个函数类型
//2如何定义一个函数指针类型
//3如何定义一个 函数指针 指向一个函数入口地址

int add(int a, int b)
{
	printf("add:...\n");
	return a + b;
}

void main()
{
	add(1, 2);//直接调用法，函数名add就是函数的入口地址
	{
		//间接调用法
		//定义一个函数类型
		typedef int(MyFuncType)(int a, int b);//定义一个函数类型
		MyFuncType *myPointerFunc = NULL;//定义了一个指针 指向了某一类函数
		myPointerFunc = &add;//也可以myPointerFunc = add;这样写 C是逐渐完善起来的

		myPointerFunc(3, 4);

	}
	{
		//定义一个函数指针类型
		typedef int(*MyPointerFuncType)(int a, int b);
		MyPointerFuncType myPointer;
		myPointer = add;
		myPointer(5, 6);
	}
	
	{
		//直接定义函数指针
		int(*MyPonterFunc)(int a, int b);//定义一个函数变量
		MyPonterFunc = add;
		MyPonterFunc(7, 8);
	}
	system("pause");
}