//左右移 double加 ++ += 都不知不觉的运用了重载机制
#include <iostream>
using namespace std;

class Complex
{
private:
	int a;
	int b;
	friend Complex operator+(Complex &c1, Complex &c2);//友元函数的应用场景
	friend Complex & operator++(Complex &c1);
	friend Complex operator++(Complex & c1, int);
	
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
		Complex tmp(this->a - c2.a, this->b - c2.b);
		return tmp;
	}
	Complex & operator--()
	{
		this->a--;
		this->b--;
		return *this;
	}
	Complex operator--(int)
	{
		Complex tmp=*this;
		this->a--;
		this->b--;
		return tmp;
	}

private:

};

Complex operator+(Complex &c1, Complex &c2)
{
	Complex tmp(c1.a + c2.a, c1.b + c2.b);
	return tmp;
}
Complex & operator++(Complex &c1)
{
	c1.a++;
	c1.b++;
	return c1;
}

Complex operator++(Complex & c1,int )//占位语法 用以区分不同的引用
{
	//先使用 再让c1++ 
	Complex tmp = c1;
	//return c1;
	c1.a++;
	c1.b++;
	return tmp;
}
//
void main()
{
	Complex c1(1, 2), c2(3, 4);
	//全局函数法实现 运算符重载
	Complex c3 = c1 + c2;//根据这句话写 重载函数 依然是考虑函数三要素 函数名（operator）参数（引用）返回值（类）
	//即可以写出Complex operator+(Complex &c1, Complex &c2);        
	c3.printC();

	//成员函数法实现 运算符重载
	//c1.operator+(c2);   函数原型是operator+(this,c2); 左操作数c1变成this指针  而this指针在类中隐藏不用写
	//即可以写出Complex operator+(Complex &c2)
	Complex c4 = c1 - c2;
	c4.printC();

	//前置++操作符 全局法
	//++c1;  函数原型Complex & operator++(Complex &c1)
	++c1;
	c1.printC();

	//前置--操作符 成员函数法
	//c1.operator--()---->Complex & operator--()
	--c2;
	c2.printC();

	//后置++ 全局函数法
	//c1++       Complex operator++(Complex &c1) //c1属性肯定变化参数用引用//此时与前置++名字重了即引用不行（返回类型不能区分引用）
	c1++;

	//后置-- 成员函数法
	//c1--   c1.operator--()====>Complex operator--(int)
	
	system("pause");
	return;
}
//总结：运算符重载本质是一个函数 就像上面的一样