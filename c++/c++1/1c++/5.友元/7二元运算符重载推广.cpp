//左右移 double加 ++ += 都不知不觉的运用了重载机制
#include <iostream>
using namespace std;

class Complex
{
private:
	int a;
	int b;
	friend Complex operator+(Complex &c1, Complex &c2);//友元函数的应用场景
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
public:
 	Complex operator-(Complex &c2)
	{
		Complex tmp(this->a-c2.a,this->b-c2.b);
		return tmp;
	}


private:

};

Complex operator+(Complex &c1, Complex &c2)
{
	Complex tmp(c1.a + c2.a, c1.b + c2.b);
	return tmp;
}
//
void main()
{
	Complex c1(1, 2), c2(3, 4);
	//全局函数法实现 运算符重载
	Complex c3=c1+c2;//根据这句话写 重载函数 依然是考虑函数三要素 函数名（operator）参数（引用）返回值（类）
	                 //即可以写出Complex operator+(Complex &c1, Complex &c2);        

	c3.printC();

	//成员函数法实现 运算符重载
	//c1.operator+(c2);   函数原型是operator+(this,c2); 左操作数c1变成this指针  而this指针在类中隐藏不用写
	//即可以写出Complex operator+(Complex &c2)

	Complex c4 = c1 - c2;
	c4.printC();

	system("pause");
	return;
}
//总结：运算符重载本质是一个函数 就像上面的一样