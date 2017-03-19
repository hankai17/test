#include <iostream>
using namespace std;

class Test
{
public: 
	Test()
	{
		a = 0;
		b = 0;
		cout << "无参构造函数" << endl;
	}
	Test(int _a)
	{
		a = _a;
		b = 0;
		cout << "有参构造函数" << endl;
	}
	void printT()
	{
		cout << "a:" << a << " b:" << b << endl;
	}
	Test(const Test & obj)
	{
		a = obj.a + 100;
		b = obj.b + 100;
		cout << "拷贝构造函数" << endl;

	}
	~Test()
	{
		cout << "析构函数 对象生命周期结束时 c++编译器自动调用" << endl;
	}
private:
	int a;
	int b;

};
void printTest(Test t)
{
	;
}
void objmain()
{
	Test t1;//Test t2()是错误的调用方法
	t1.printT();
	Test t3(1);//括号法  编译器自动调用
	Test t4 = 4;//等号法   编译器自动调用
	Test(2);//没人接立即析构
	Test t5 = Test(5);//直接法：对象的初始化接 匿名转有名
	Test t6 = t1;//调用拷贝构造函数
	t1=Test(99);//对象接 赋值后 立即析构
	t1.printT();
	cout << "***************" << endl;
	Test t7 = Test(t6);//会调用拷贝构造函数

	return;
}

void objplay1()//拷贝构造函数 情景1,2
{
	Test t1(1);
	Test t2(t1);//拷贝构造函数 情景1
	Test t3 = t1;//拷贝构造函数 情景2
	printTest(t3);//拷贝构造函数 情景3  //实参t3初始化t 调用t拷贝构造函数
}

Test getTestObj()//返回一个类 产生匿名对象
{
	Test t(1);
	return t;
}
void TestNoName()//拷贝构造情景4
{
	Test myt1 = getTestObj();//用匿名对象初始化另一个对象
	Test myt2(1);
	myt2 = getTestObj();//用匿名对象给另一个对象赋值 匿名被析构
}
void main()
{
	objmain();
	//objplay1();
	//TestNoName();
	system("pause");
	return;

}