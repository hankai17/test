#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int myadd(int a, int b)//子任务实现者
{
	printf("func add() do...\n");
	return a + b;
}
int myadd2(int a, int b)
{
	printf("func add2() do...\n");
	return a + b;
}
int myadd3(int a, int b)
{
	printf("func add3() do...\n");
	return a + b;
}
int myadd4(int a, int b)
{
	printf("func add4() do...\n");
	return a + b;
}

typedef int(*MyTypeFuncAdd)(int a, int b);//！！！！！！！！！！！返回值是 int 参数是 a ,b!!!!!!
                                           //函数指针类型起到的作用：把函数参数 返回值做了约定  这是回调的本质
int MainOp(MyTypeFuncAdd myFuncAdd)//或者 int MainOp(int(*MyTypeFuncAdd)(int a, int b))
{
	int c=myFuncAdd(5, 6);
	return c;
}

int MainOp2(int(*MyPointerFuncAdd)(int a, int b))
{
	int c = MyPointerFuncAdd(5, 6);
	return c;
}

void main()//完成了任务调用 任务的编写可以分开
{
	//myadd(1, 2);//直接调用
	MyTypeFuncAdd myFuncAdd = myadd;//间接调用
	//myFuncAdd(3, 4);

	MainOp2(myadd);
	MainOp(myadd);
	
	MainOp(myadd2);//在mainop框架 没有发生任何变化的情况下 可以把后来写的代码调用起来
	MainOp(myadd3);//函数指针（函数入口地址）做函数参数 跟多态一模一样
	MainOp(myadd4);
	system("pause");
	return;
}
//xwq18738834376
//任务的调用者和任务的编写者 实现了分离
//任务先写好 供给别人 调用 就是回调