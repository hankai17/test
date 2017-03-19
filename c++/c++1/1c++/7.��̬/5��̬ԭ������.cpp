#include <iostream>
using namespace std;
//多态成立的三个条件 继承 虚函数重写 父类指针指向子类对象
class Parent
{
public:
	Parent(int a = 0)
	{
		this->a = a;
	}
	virtual void print()//虚函数重写
	{
		cout << "我是爹" << endl;
	}
protected:
private:
	int a;
};

class Child:public Parent
{
public:
	Child(int b = 0)
	{
		this->b = b;
	}
	virtual void print()//虚函数重写 可加可不加
	{
		cout << "我是孩子" << endl;
	}
protected:
private:
	int b;
};

void HowToPlay(Parent * base)
{
	base->print();//会有多态发生
}//C++编译器根本不需要区分是子类还是父类 两zhe都有vpr指针 找各自虚函数表 再找各函数的入口地址

void main()
{
	Parent p1;//用类定义对象的时候 编译器偷偷的加了一个vpr指针 
	Child c1;

	HowToPlay(&p1);
	HowToPlay(&c1);

	system("pause");
	return;
}