#include <iostream>
using namespace std;

class A
{
public:
	int a;
	int b;
	void getB()
	{
		cout << "b "<<b << endl;
	}
	void print()
	{
		cout << "AAAA" << endl;
	}
protected:
private:

};

class B:public A
{
public:
	int b;
	int c;
	void get_child()
	{
		cout << "b " << b << endl;
	}
	void print()
	{
		cout << "BBBB" << endl;
	}
protected:
private:

};


void main1()
{
	A a1;
	B b1;
	b1.b = 1;//或者b1.B::b=1
	b1.get_child();//默认调用子类B的b 

	b1.A::b = 100;
	b1.getB();//调用父类的 

	system("pause");
	return;
}
void main()
{
	B b1;
	b1.print();//默认调用子类的成员函数
	b1.A::print();//调用父类

	system("pause");
	return;
}