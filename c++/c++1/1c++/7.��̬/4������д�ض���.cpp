#include <iostream>
using namespace std;

class Parent
{
public:
	virtual void func()
	{
		cout << "func() do..." << endl;
	}
	virtual void func(int i)
	{
		cout << "func() do..." << endl;
	}
	virtual void func(int i, int j)
	{
		cout << "func() do..." << endl;
	}
protected:
private:

};

class Child :public Parent
{
public:
	virtual void func(int i, int j)
	{
		cout << "func() do..." << endl;
	}
	virtual void func(int i,int j,int k )
	{
		cout << "func() do..." << endl;
	}
protected:
private:
};


void main()
{
	Child c1;
	//c1.func(); 子类无法重载父类的函数 如果子类与父类的函数名相同 则名称覆盖不会发生重载 
	c1.Parent::func();//只能用域作用符
	c1.func(1, 2);//c++编译器 看到func名字 在子类中已经存在了 所以编译器不会找父类的4个参数的func函数
	              //编译器只会在子类中 查找func函数 找到了两个func 一个是两个参数的 一个是3个参数的
	              //如果想调用父类的函数 必须显示调用即域作用符法
	system("pause");
	return;
}
//重载 必须在同一个类中进行
//重写 父类子类之间 一模一样的函数 1虚函数重写（发生多态） 2非虚函数重写（重定义）