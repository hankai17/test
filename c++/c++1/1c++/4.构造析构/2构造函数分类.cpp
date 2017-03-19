#include <iostream>
using namespace std;

class Test2
{
public:
	Test2()
	{
		cout << "无参数构造函数" << endl;
	}
	Test2(int a,int b)
	{
		m_a = a;
		m_b = b;
		cout << "有参数构造函数" << endl;

	}
	Test2(const Test2 & obj)//赋值构造函数(拷贝构造函数)
	{
		cout << "我也是构造函数" << endl;
	}
	Test2(int a)
	{
		m_a = a;
		m_b = 0;
	}
	~Test2()
	{
		cout << "析构函数" << endl;
	}
private:
	int m_a;
	int m_b;
public:
	void printT()
	{
		cout << "普通成员函数" << endl;
	}
};

void main1()
{
	Test2 t1;//调用无参构造函数
	system("pause");
	return;
}
void main2()//括号法,必须括号里面有内容
{
	Test2 t1 (1,2);//调用有参构造函数
	t1.printT();
	system("pause");
	return;
}

void main3()//等号法
{
	Test2 t2=(1,2,3,4,5,5);//调用有参构造函数,调用的是有一个参数的那个函数 此等号非C得等号 C++对此进行功能增强
	               // f9 f11
	
	system("pause");
	return;
}
//以上都是c++编译器自动调用的构造函数
void main()//手工调用 即直接调用法 会产生匿名对象
{
	Test2 t4 = Test2(1, 2);//完成对t4的初始化
	Test2 t5;
	t5 = Test2();
	//t1 = t4;  // 这是赋值 初始化不等于赋值
	system("pause");
	return;
}

//构造函数的编写是个人写的  而且自动调用（隐式的调用）而显示的调用容易忘而且麻烦 