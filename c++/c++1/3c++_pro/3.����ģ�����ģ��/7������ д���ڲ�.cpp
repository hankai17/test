#include <iostream>
using namespace std;

template <typename T>
class Complex
{
	friend ostream & operator << (ostream &out, Complex &c3)
	{
		out << "a:" << c3.a << "b:" << c3.b << endl;
		return out;
	}
	
public:
	Complex(T a , T b )
	{
		this->a = a;
		this->b = b;
	}
	void printCom()
	{
		cout << "a:" << a << "b:" << endl;
	}
	Complex operator+ (Complex &c2)//！！！！！！！！！！！！！！！！！！！！！写里面；。练习册；LKJHGFMN   
	{
		Complex tmp(a + c2.a , b + c2.b);
		return tmp;
	}
	
protected:
private:
	T a;
	T b;

};


/*
ostream & operator << (ostream &out, Complex &c3)//所有函数均写在内部

{
	out << "a:" << c3.a << "b:" << c3.b << endl;
	return out;
}
*/
void main()
{
	Complex<int> c1(1, 2);//需要把模板类具体化以后才能定义对象
	Complex <int>c2(3, 4);
	Complex<int> c3 = c1 + c2;//c1是左操作数通过this指针隐藏
	//Complex operator+(complex &c2);
	//c3.printCom();

	cout << c3 << endl;//<<>>只能用友元函数 其他符号重载都写成成员函数 不要滥用友元函数！！！！！！！！！！！！！！！！！！！
	//ostream & operator << (ostream &out, Complex &c3)//返回一个引用以支持链式编程
	//cout << c3 << endl;

	system("pause");
	return;
}

















/*
#include <iostream>
using namespace std;




class Complex
{
	friend ostream & operator << (ostream &out, Complex &c3);
public:
	Complex(int a = 0, int b = 0)
	{
		this->a = a;
		this->b = b;
	}
	void printCom()
	{
		cout << "a:" << a << "b:" << endl;
	}
	Complex operator+(Complex &c2)//！！！！！！！！！！！！！！！！！！！！！写里面；。练习册；LKJHGFMN   
	{
		Complex tmp(a + c2.a + b + c2.b);
		return tmp;
	}
protected:
private:
	int a;
	int b; 

};



ostream & operator << (ostream &out, Complex &c3)
{
	out << "a:" << c3.a << "b:" << c3.b << endl;
	return out;
}
void main()
{
	Complex c1(1, 2);
	Complex c2(3, 4);
	Complex c3 = c1 + c2;//c1是左操作数通过this指针隐藏
	//Complex operator+(complex &c2);
	c3.printCom();
	
	//cout << c3 << endl;
	//ostream & operator << (ostream &out, Complex &c3)//返回一个引用以支持链式编程
	cout << c3 << endl;

	system("pause");
	return;
}
*/