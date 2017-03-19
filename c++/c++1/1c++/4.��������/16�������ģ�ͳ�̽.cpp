//非常重要！！！！！！
//编译器支持的各种机制
//封装 把数据和函数扭在一起
//编译器如何管理 类 对象 类和对象的关系的
#include <iostream>
using namespace std;

class C1
{
public:
	int i;
	int j;
	int k;
private:

};

class C2
{
public:
	int i;
	int j;
	int k;
	static int m;
public:
	int getK() const { return k; }//  函数的入口地址 也是4个字节
	void setK(int val){ k = val; }

private:
	

};                                //24 16

struct S1
{
	int i;
	//int j;
	//int k;
};

struct S2
{
	int i;
	int j;
	int k;
	static int m;
};


void main()
{
	printf("c1:%d\n", sizeof(C1));//12
	printf("c2:%d\n", sizeof(C2));//12
	printf("s1:%d\n", sizeof(S1));//12
	printf("s2:%d\n", sizeof(S2));//12
	



	system("pause");
	return;
}
//c++中成员函数（代码区） 与成员变量（栈区）时分开存储的  this指针！！
//c++中类的普通成员函数都隐式的包含了一个指向 当前对象 的this指针
//所以不怕多个对象 调用同样的函数（代码）