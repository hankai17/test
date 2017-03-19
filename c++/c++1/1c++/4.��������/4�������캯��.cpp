#include <iostream>
using namespace std;

class Test4
{
public:
	Test4()
	{
		cout << "无参数构造函数" << endl;
	}
	Test4(int a, int b)
	{
		m_a = a;
		m_b = b;
		cout << "有参数构造函数" << endl;

	}
	Test4(const Test4 & obj)//赋值构造函数(拷贝构造函数)
	{
		cout << "我是copy构造函数" << endl;
		m_b = obj.m_b+100;
		m_a = obj.m_a+100;
	}
	Test4(int a)
	{
		m_a = a;
		m_b = 0;
	}
public:
	void printT()
	{
		cout << "普通成员函数" << endl;
		cout << "m_a " << m_a <<"m_b "<<m_b<< endl;
	}
	~Test4()
	{
		cout << "析构函数" << endl;
	}
private:
	int m_a;
	int m_b;

};
//copy构造函数： 用一个对象初始化另一个对象

void main()
{
	Test4 t1(1, 2);
	Test4 t0(1, 2);
	
	t0 = t1;//赋值操作 不会调用拷贝构造函数
	//第一种调用方法
	Test4 t2 = t1;//t1初始化t2 初始化操作
	t2.printT();

	//system("pause");
	return;
}

void main2()//第二种调用方法
{
	Test4 t1(1, 2);
	Test4 t0(1, 2);

	Test4 t2(t1);//用t1初始化 t2
	t2.printT();
	system("pause");
	return;
}

