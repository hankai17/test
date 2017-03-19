#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

class A
{
public:
	A()
	{
		p = new char[20];
		strcpy(p, "obj");
		printf("A()\n");
	}
	virtual ~A()
	{
		delete[] p;
		printf("~A()\n");
	}
protected:
private:
	char * p;

};

class B:public A
{
public:
	B()
	{
		p = new char[20];
		strcpy(p, "obj");
		printf("B()\n");
	}
	virtual ~B()
	{
		delete[] p;
		printf("~B()\n");
	}
protected:
private:
	char * p;

};

class C:public B
{
public:
	C()
	{
		p = new char[20];
		strcpy(p, "obj");
		printf("C()\n");
	}
	virtual ~C()
	{
		delete[] p;
		printf("~C()\n");
	}
protected:
private:
	char * p;

};

void howtodelete(A *base)//父子兼容性原则
{
	delete base;//这句话不会表现成多态 只调用父类的函数//如果想通过基类指针释放所有子类资源（析构函数） 则需要虚析构函数
}
void main()
{
	C *myC = new C;
	howtodelete(myC);//只执行了父类的析构函数 有内存泄露//或者 delete myC这种方式 不用virtual

	system("pause");
	return;
}