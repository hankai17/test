#include <iostream>
using namespace std;

class Test
{
public:
	int a;
	int b;
public:
	void printT()
	{
		cout << "a" << a << "  b" << b << endl;
	}
private:
public:
	Test(int a=0, int b=0)
	{
		cout << "构造函数" << endl;
		this->a = a;
		this->b = b;
	}
	Test TestAdd(Test &t2)
	{
		Test tmp(this->a+t2.a,this->b+t2.b);
		return tmp;//返回元素会拷贝成匿名对象

	}
	~Test()
	{
		cout << "析构函数" << endl;
	}
	Test & TestAdd2(Test &t2)//返回一个引用 相当于返回自身（内存首地址） 返回的是t1这个对象
		                       //this就是t1的地址 所以返回的就是t1
	{
		this->a = this->a + t2.a;
		this->b = this->b + t2.b;
		return *this;//t1.TestAdd2(t2)
	}
};

Test TestAdd(Test &t1, Test &t2)//全局函数
{
	Test tmp;
	return tmp;

}
void main1()
{
	Test t1(1, 2);
	Test t2(3, 4);

	Test t3;

	t3 = TestAdd(t1, t2);//全局函数法
	//t3=t1.TestAdd(t2);//t1.TestAdd(t1, t2);//面向对象没学好
	

	Test t4 = t1.TestAdd(t2);//匿名对象的接 法
	t4.printT();
	
	Test t5;
	t5 = t1.TestAdd(t2);//匿名对象的另一种接法
	t5.printT();

	system("pause");
	return;
}
//全局函数转成员函数 少了一个参数  体现了面向对象思想 很方便


void main()
{
	Test t1(1, 2);
	Test t2(3, 4);

	t1.TestAdd2(t2);//t1=t1+t2
	t1.printT();
	system("pause");

}