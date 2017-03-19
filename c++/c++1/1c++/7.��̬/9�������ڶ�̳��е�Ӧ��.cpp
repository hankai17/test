#include <iostream>
using namespace std;

class Interface1
{
public:
	virtual int add(int a, int b) = 0;
	virtual void print() = 0;
protected:
private:

};

class Interface2
{
public:
	virtual int mult(int a, int b) = 0;
	virtual void print() = 0;
protected:
private:

};

class Parent
{
public:
	int getA()
	{
		a = 0;
		return a;
	}
private:
	int a;
};

class Child :public Parent, public Interface1, public Interface2//实现虚函数重写
{
public:
	virtual int add(int a, int b)
	{
		cout << "Child: add()执行\n" << endl;
		return a+b;
	}
	virtual void print() 
	{
		cout << "Child: print()执行\n" << endl;
	}
	virtual int mult(int a, int b) 
	{
		cout << "Child: mult()执行\n" << endl;
		return a*b;
	}
private:

};

void main()
{
	Child c1;
	Interface1 *it1 = &c1;
	it1->add(1, 2);

	Interface2 *it2 = &c1;
	it2->mult(3, 6);

	system("pause");
	return;
}