#include <iostream>
using namespace std;

class Test3
{
public:
	Test3(int a = 0, int b = 0)
	{
		this->a = a;
		this->b = b;
	}
	~Test3()
	{
		cout << "析构函数\n" << endl;
	}
private:
	int a;
	int b;
};

void myDivide()//void myDivide() throw(int,char,char *)//可以扔出int char char *类型的异常
               //void myDivide() //可以抛出任何类型异常
			   //void myDivide() throw()//不抛出异常     。。。。。。。。。。。。这就是异常接口声明
{
	Test3 t1(1, 2), t2(3, 4);//定义两个栈变量 当发生异常时 会解旋
	cout << "mydivide..要发生异常\n";
	throw 1;
}


void main()
{
	try
	{
		myDivide();
	}
	catch (int a)
	{
		cout << "int 类型异常\n";
	}
	catch (...)
	{
		cout << "未知异常\n";
	}

	system("pause");
	return;
}

