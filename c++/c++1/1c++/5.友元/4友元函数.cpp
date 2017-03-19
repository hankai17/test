//可以修改 类私有属性
//友元函数 至少包含类指针和一个值 

#include <iostream>
using namespace std;

class  A
{
	//声明的位置和public private没有关系
	friend void modifyA(A * pA, int _a);//函数  是类A的好朋友
public:
	
	A(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	int getA()
	{
		return this->a;

	}
private:
	int a;
	int b;
};

void modifyA(A * pA,int _a)
{
	//pA->a = 100;
	pA->a = _a;
}

void main()
{
	A a1(1, 2);
	cout << a1.getA() << endl;
	modifyA(&a1, 300);
	cout << a1.getA() << endl;

	system("pause");
	return;

}