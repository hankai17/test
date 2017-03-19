#include <iostream>
using namespace std;

int getA1()
{
	int a;
	a = 10;
	return a;
}

int& getA2()//返回值是引用
{
	int a;
	a = 10;
	return a;//返回的是一个内存空间a(就是返回10)
}

int * getA3()//返回值是指针
{
	int a;
	a = 10;
	return &a;
}



void main1()
{
	int a1 = 0;
	int a2 = 0;
    a1 = getA1();//返回10 所以把10赋值给a1 ，由于内存被析构 10放到寄存器里了
	
	a2 = getA2();//返回a c++编译器一看a是引用 自动变为*a 由于内存析构掉了 10存放在寄存器里
	
	int &a3 = getA2();//返回10 但引用来接
	                //getA2()自身是地址 就相当于a3存放的是个地址 c++编译器一看a3是引用 自动变为*a3 但是栈已经清空了 返回的应该是乱码
					//返回栈上的引用 可能会有问题
	printf("a1:%d a2:%d a3:%d\n", a1, a2, a3);//a3应该是个乱码

	system("pause");
	return;
}

int j1()
{
	static int a = 10;//静态变量
	a++;
	return a;
}

int & j2()
{
	static int a = 10;//静态变量
	a++;
	return a;
}

void main2()
{
	int a1 = 10;
	int a2 = 10;

	a1 = j1();
	a2 = j2();
	int &a3 = j2();//若返回静态变量或全局变量可以成为其他引用的初始值（可作为左值右值）

	printf("a1:%d a2:%d a3:%d", a1, a2, a3);
	system("pause");
	return;
}

//函数当左值
int g1()
{
	static int a = 10;
	a++;
	return a;
}//返回变量的值
int & g2()
{
	static int a = 10;
	a++;
	printf("a:%d \n", a);
	return a;
}//返回变量本身a，返回变量的内存空间
void main()
{
	//g1() = 100;
	g2() = 100;//函数返回值是一个引用 并且当左值

	g2();
	system("pause");
}
//返回栈上的引用，用引用来接，出现乱码；返回静态引用，引用来接，正常。还是内存的问题。
//用变量来接非常简单。但是用引用来接 首先要注意：这个变量在哪里？是栈还是静态区？
//返回值是引用，返回的是一个内存空间；而通常返回值是一个常量。
