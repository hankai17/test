#include <iostream>
using namespace std;

class Figure//抽象类  不能被实例化
{
public:
	//约定一个统一接口（界面） 让子类必须实行
	virtual void getArea() = 0;//纯虚函数 目的是让子函数继承
protected:
private:

};


class Circle :public Figure
{
public:
	Circle(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	virtual void getArea()
	{
		cout << "圆的面积：" << 3.14*a*a << endl;
	}

private:
	int a;
	int b;
};

class Tri :public Figure
{
public:
	Tri(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	virtual void getArea()
	{
		cout << "三角形的面积：" << 0.5*a*b << endl;
	}

private:
	int a;
	int b;
};

class Square :public Figure
{
public:
	Square(int a, int b)
	{
		this->a = a;
		this->b = b;
	}
	virtual void getArea()
	{
		cout << "长方形的面积：" << a*b << endl;
	}

private:
	int a;
	int b;
};

void objplay(Figure * base)
{
	base->getArea();

}
void main()
{
	//Figer f;不能被实例化
	//Figure * base = NULL;//可定义指针
	Circle c1(10,20);//c1.getArea();//没有用多态
	Tri t1(20, 30);
	Square s1(3, 4);

	//面向抽象类编程 面向一套预先定义好的接口编程  解耦合  模块的划分

	objplay(&c1);
	objplay(&t1);
	objplay(&s1);
	system("pause");
	return;
}