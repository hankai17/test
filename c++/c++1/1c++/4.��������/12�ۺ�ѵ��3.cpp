#include <iostream>
using namespace std;

class MyTest
{
public:
	MyTest(int a, int b, int c)
	{
		this->a = a;
		this->b = b;
		this->c = c;

	}
	MyTest(int a, int b)
	{
		this->a = a;
		this->b = b;
		MyTest(a, b, 100);//构造函数里调用构造函数
	}
	~MyTest()
	{
		printf("MyTest~:%d,%d,%d\n", a, b, c);
	}

private:
	int a;
	int b;
	int c;

public:
	int getC() const { return c; }
	void setC(int val) { c= val; }
};

void main()
{
	MyTest t1(1, 2);// 调用MyTest(int a, int b)，其中直接使用构造函数 MyTest(a, b, 100);会产生匿名对象1 2 100  但是没人接立即析构
	printf("c:%d\n", t1.getC());//c是乱码
	system("pause");
	return;
}
//在构造函数中调用构造函数 是个危险的行为