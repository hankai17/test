#include <iostream>
using namespace std;

class A
{
public:
	static int a;
	int b;
	void get()
	{
		cout << "b " << b << endl;
	}
	void print()
	{
		cout << "AAAA" << endl;
	}
	A()
	{
		cout << "A的构造函数" << endl;
	}
protected:
private:

};

int A::a = 100;//静态成员变量初始化 分配内存！！

class B :private A
{
public:
	int b;
	int c;
	void get_child()
	{
		cout << "b " << b << endl;
		cout << a << endl;//static遵循 派生类的访问控制规则
	}
	void print()
	{
		cout << "BBBB" << endl;
	}
protected:
private:

};
void main1()//static遵循 派生类的访问控制规则
{
	B b1;  
	//b1.a=100;外部不行 static遵循 派生类的访问控制规则

	system("pause");
}

void main()
{
	A a1;
	a1.print();

	B b1;
	b1.get_child();
	system("pause");
}
