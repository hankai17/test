#include <iostream>
using namespace std;

class Parent
{
public:
	void print()
	{
		a = 0;
		b = 0;
		cout << "a " << a << endl;
		cout << "b " << b << endl;
	}
public:
	int a;
	int b;
};

//class Child:public Parent
//class Child :private Parent
class Child :public Parent
{
public:
protected:
private:
	int c;
};

void main()
{
	Child c1;
	c1.a = 1;
	c1.b = 3;
	c1.print();
	system("pause");
	return;
}