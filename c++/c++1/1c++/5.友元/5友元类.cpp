#include <iostream>
using namespace std;

class A
{
	friend class B;//b是A的好朋友 即在B中可以访问A的私有属性
public:
	
private:
	int a;

};

class B
{
public :
	void Set(int a){ Aobject.a = a; }
	void printB(){ cout << Aobject.a << endl; }
private:
	A Aobject;
};

void main()
{
	B b1;
	b1.Set(300);
	b1.printB();

	system("pause");
	return;
}


//友元类 不常用因为破坏了类内部封装性
//友元函数常用