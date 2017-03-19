//引用必须要赋值才能用，就像用const定义一个常量一样必须要赋值 所以引用很像一个常量
//一个内存可有不同的门牌号（a或b）
//定义一个 int &a； 占4个字节 很像指针所占内存大小  即使是double int a仍占4个字节
//由上可得：常量+指针:int * const a。就是 int &a。*a=5就是 a=5	


#include <iostream>

using namespace std;

void modifyA1(int &a1)//a1内部是10
{
	a1 = 10;
}

void modifyA2(int * const a1)
{
	*a1 = 100;
}

void main1()
{
	int a = 10;
	modifyA1(a);//c++编译器自动取了一个地址给a，就是a是一个地址 ，地址内存的是10
	printf("a: %d\n",a);

	modifyA2(&a);//需要我们手动取地址
	printf("a: %d", a);
	system("pause");
	return;
}

void modifyA3(int * p)
{
	*p = 201;
}
void main()//间接赋值三个条件：两个变量+建立关联+*p
{
	int a = 10;
	int *p = NULL;

	p = &a;
	*p = 100;

	modifyA3(&a);

	printf("a=%d ", a);
	system("pause");
	return;
}
//当我们用引用时 不必关心引用的具体内部编译器的细节