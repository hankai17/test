#include <iostream>
using namespace std;

class Base1
{
public:
	Base1(int b1)
	{
		this->b1 = b1;
	}
	void printB1()
	{
		cout << "b1 " << b1 << endl;
	}
protected:
private:
	int b1;
};

class Base2
{
public:
	Base2(int b2)
	{
		this->b2 = b2;
	}
	void printB2()
	{
		cout << "b2 " << b2 << endl;
	}
protected:
private:
	int b2;
};

class B:public Base1,public Base2
{
public:
	B(int b1, int b2, int c) :Base1(b1), Base2(b2)
	{
		this->c = c;

	}
	void printC()
	{
		cout << "c " << c << endl;
	}

protected:
private:
	int c;
};
void main()
{
	B b1(1, 2, 3);
	b1.printC();
	b1.printB1();
	b1.printB2();

	system("pause");
	return;
}