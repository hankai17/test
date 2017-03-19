//面向对象三大概念三大境界 突破函数概念 直接复用别人写的代码 能继承复用别人的代码还能拓展后人写的代码
#include <iostream>
using namespace std;

class Parent
{
public:
	Parent(int a)
	{
		this->a = a;
		cout << " Parent a"<<a << endl;
	}
	virtual void print()
	{
		cout << "Parent 打印 a" << endl;
	}
protected:
private:
	int a;

};
class Child :public Parent
{
public:
	Child(int b) :Parent(10)
	{
		this->b = b;
		cout << "Child b" << b << endl;
	}
	void print()
	{
		cout << "Child 打印 b" << endl;
	}
private:
	int b;
};

void howToPrint(Parent * base)
{
	base->print();
}

void howToPrint2(Parent & base)
{
	base.print();
}

void main()
{
	Parent *base = NULL;
	Parent p1(20);
	Child c1(30);
	/*
	base = &p1;
	base->print();//父类打印函数

	base = &c1;//类型兼容性原则
	base->print();//执行父类打印函数//面向对象新需求
	*/
	/*
	Parent &base2 = p1;//引用
	base2.print();//打印父类
	Parent &base3 = c1;//引用
	base2.print();//仍打印父类
	*/
	
	howToPrint(&p1);
	howToPrint(&c1);//父类
	howToPrint2(p1);
	howToPrint2(c1);//父类

	//virtual 父类写了，子类可写可不写一般写上

	system("pause");
	return;
}
//一个调用语句base->print(); 有多重表现形态 就是多态 