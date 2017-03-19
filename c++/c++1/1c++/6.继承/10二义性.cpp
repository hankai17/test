#include <iostream>
using namespace std;

class B
{
public:
	int b;
	
protected:
private:
	
};

class B1:virtual public B
{
public:
	int b1;
protected:
private:
};

class B2 :virtual public B
{
public:
	int b2;
protected:
private:
};

class C :public B1, public B2
{
public:
	int c;

protected:
private:
};
void main()
{
	C c1;
	c1.b1 = 100;
	c1.b2 = 99;
	c1.c = 9;
	//c1.b = 0; 是从b1取b还是从b2取b 由此出现虚继承 virtual
	 c1.b = 0;


	system("pause");
	return;
}

//实际中 不需要多继承