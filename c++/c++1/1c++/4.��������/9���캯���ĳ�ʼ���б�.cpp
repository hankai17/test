#include <iostream>
using namespace std;

class A
{
public:
	A(int _a)
	{
		a = _a;
		cout << "构造函数  a" << endl;
	}
	~A()
	{
		
		cout << "析构函数 a" << endl;
	}
private:
	int a;

};

class B//在B类中组合了一个A类对象 A类设计了构造函数
	    //根据构造函数调用规则 写了A类构造函数 必须要用 没有机会初始化A 新的语法出现了
{
public:
	B(int _b1, int _b2) :a1(1), a2(2) , c(0)
	{
		
	}
	B(int _b1, int _b2, int m, int n) :a1(m), a2(n) , c(0)
	{
		b1 = _b1;
		b2 = _b2;
		cout << "B的构造函数" << endl;
	}
	~B()
	{
		cout << "B的析构函数" << endl;
	}
private:
	int b1, b2;
	A a1; 
	A a2;
	const int c;//如果有const属性 必须要初始化
};

void objplay()//先执行被组合对象的构造函数 如果组合对象有多个按照定义顺序 而不是按照初始化列表的顺序
           //析构函数和构造函数的调用顺序相反
{
	//A a1(10);
	//B objB（1,2）;

	B objB2(1, 2, 3, 4);//先执行A类的构造函数 再执行B的构造函数
	
}

void main()
{
	objplay();
	system("pause");
	return;
}