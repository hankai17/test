//用一个函数名定义不同的函数 
//当函数名和不同的参数搭配时函数的含义不同
//名称 参数 返回值
//名称相同 参数不一样（个数 类型）
//返回值不是判断函数重载的标准
//调用标准：正常人思维
#include <iostream>
using namespace std;

void myPrint(int a)
{ 
	printf("a:%d\n", a);
}

void myPrint(char *p)
{
	printf("p:%s\n", p);
}

void myPrint(int a,int b)
{
	printf("a:%d\n", a);
	printf("b:%d\n", b);
}
/*
int myPrint(int a, int b)   //返回值不是判断函数重载的标准
{
	printf("a:%d\n", a);
	printf("b:%d\n", b);
}
*/
void main1()
{
	myPrint(1);//匹配第一个函数
	myPrint("122121212aa");//匹配第二个函数
	myPrint(1, 2);//匹配第三个函数
	
	system("pause");
}
//函数默认参数和函数重载在一起
void myfunc(int a, int b, int c = 0)
{
	printf("a:%d b:%d c:%d\n",a,b,c);
}

void myfunc(int a, int b)
{
	printf("a:%d b:%d\n", a, b);
}
void myfunc(int a)
{
	printf("a:%d \n", a);
}
void main()
{
	//myfunc(1,2);//函数调用时候会产生二义性
	myfunc(1);
	system("pause");
}
//函数重载与函数指针在一起
