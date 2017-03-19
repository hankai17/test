//比较难的语法
#include <iostream>
using namespace std;

void myfunc(int a)
{
	printf("a:%d\n", a);
}

void myfunc(int *p)
{
	printf("%s\n", p);
}

void myfunc(int a,int b)
{
	printf("a:%d\n", a);
}

void myfunc(char * a, char * b)
{
	printf("a:%s\n", a);
	printf("b:%s\n", b);
}

//函数指针
//声明一个函数类型
//声明一个函数指针类型
//定义函数指针变量

//void myfunc(int a, int b)
typedef void (myTypeFunc)(int a, int b);  //声明了一个类型myTypeFunc用 来定义函数指针
//myTypeFunc *myfuncp = NULL;//定义一个函数指针（必须的） 这个指针指向函数入口地址
typedef void (*myPTypeFunc)(int a, int b); //声明了一个指针数据类型 用来定义函数指针
//myTypeFunc fp = NULL;//通过函数指针类型定义一个指针
void(*myVarPFunc)(int a, int b);//直接定义一个函数指针变量

void main()
{
	myPTypeFunc fp;//
	fp = myfunc;
	fp(1,2);//在此处才会检测出到底用哪个函数的入口
	
	/*
	char buf1[] = "aaaaa";//报错 buf1 2必须是int类型
	char buf2[] = "bbbb";
	fp(buf1, buf2);
	*/ 
	
	
	system("pause");
	return;
}
