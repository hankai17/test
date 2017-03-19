#include <iostream>
using namespace std;
//多态成立的三个条件 继承 虚函数重写 父类指针指向子类对象
class Parent
{
public:
	Parent(int a = 0)
	{
		this->a = a;
		print();//调哪个？  虽然在初始化子类 但调用的是父类的 vptr分步初始化
	}
	virtual void print()//虚函数重写
	{
		cout << "我是爹" << endl;
	}
protected:
private:
	int a;
};

class Child :public Parent
{
public:
	Child(int a = 0, int b = 0) :Parent(a)
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
	base->print();
}

void main()
{
	Child c1;
	//c1.print();

	system("pause");
	return;
}