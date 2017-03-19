#include <iostream>
using namespace std;

class Parent
{
public:
	void printP()
	{
		cout << "我是爹" << endl;
	}
	Parent()
	{
		cout << "构造函数" << endl;
	}
	Parent(const Parent & obj)
	{
		cout << "copy构造函数" << endl;
	}
private:
	int a;

};

class Child :public Parent
{
public:
	void printC()
	{
		cout << "我是儿子" << endl;
	}

};
void howToPrint(Parent * base)
{
	base->printP();
}

void howToPrint2(Parent & base)
{
	base.printP();
}
void main()
{
	Parent p1;
	p1.printP();

	Child c1;
	c1.printP();

	Parent *p = NULL;//1.1基类指针（引用）指向子类对象
	p = &c1;
	p->printP();

	//1.2指针做函数参数
	howToPrint(&p1);
	howToPrint(&c1);
	
	//1.3引用做函数参数
	howToPrint2(p1);
	howToPrint2(c1);

	//用父类对象初始化子类对象 因为子类是一种特殊父类
	//2.1
	Parent p3 = c1;//会调用拷贝构造函数


	system("pause");
	return;
}