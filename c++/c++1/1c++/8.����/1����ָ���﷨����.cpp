#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//定义一个数组类型

//定义一个指针数组类型

//定义一个指向 数组类的指针 数组类的指针

void main()
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