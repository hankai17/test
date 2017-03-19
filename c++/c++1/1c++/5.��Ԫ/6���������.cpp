#include <iostream>
using namespace std;

class Complex
{
public:
	int a;
	int b;
public:
	Complex(int a = 0, int b = 0)
	{
		this->a = a;
		this->b = b;
	}
	void printC()
	{
		cout << a << " + " << b << "i" << endl;
	}
private:

};
//第一步 定义一个全局函数
Complex myAdd(Complex &c1, Complex &c2)
{
	Complex tmp(c1.a+c2.a,c1.b+c2.b );
	return tmp;
}
//第二部 改造函数名
Complex operator+(Complex &c1, Complex &c2)
{
	Complex tmp(c1.a + c2.a, c1.b + c2.b);
	return tmp;
}
//
void main()
{
	int a = 0, b = 0;
	int c;
	c = a + b;//编译器知道了如何运算

	// a+bi 复数
	Complex c1(1, 2), c2(3, 4);
	Complex c3;//用户自定义数据类型 编译器不知道如何运算
	//c3 = c1 + c2; 编译器应该提供一种机制 让用户有机会最自定义数据类型 有机会进行运算符操作
	
	Complex c4=myAdd(c1, c2);
	c4.printC();

	Complex c5 = operator+(c1, c2);
	c5.printC();

	Complex c6 = c1+ c2;//调用那个函数
	c6.printC();


	system("pause");
	return;
}
//总结：运算符重载本质是一个函数 就像上面的一样