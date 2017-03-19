#include <iostream>
using namespace std;

class Test
{
public:
	Test(int a, int b)// c++编译器这样做 Test(Test * this, int a,int b)  this 就是t1的地址 谁调它 它就是谁的地址
	{
		this->a = a;//否则会产生歧义
		this->b = b;
	}
	void printT()
	{
		cout << "a: " << a << endl;
		cout << "b: " << this->b<< endl;
	}
private:
	int a;
	int b;
};

void main()
{
	Test t1(1, 2);
	t1.printT();//编译器 printT(&t1)

	system("pause");
	return;
}