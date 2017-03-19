#include <iostream>
using namespace std;

//public修饰的成员变量 方法在类的内外都能使用。
//private:private继承后只能在类内部使用
//protected：protect继承以后只在类的内部使用 在继承的子类中可用
class Parent
{
public:
	int a;//老王的名字
protected:
	int b;//老王的密码
private:
	int c;//老王的情人
public:
	void printT()
	{
		cout << "printT" << endl;
	}

};

class Child :public Parent//public继承
{
public:
	void useVar()
	{
		a = 0;
		b = 0;
		//c = 0;
	}
protected:
private:

};

class Child2 :private Parent//私有继承
{
public:
	void useVar()
	{
		a = 0;
		b = 0;
		//c = 0;
	}
protected:
private:

};

class Child3 :protected Parent//私有继承
{
public:
	void useVar()
	{
		a = 0;//protected
		b = 0;//protected
		//c = 0;
	}
protected:
private:

};

void main1()
{
	Parent t1, t2;
	t1.a = 10;
	//t1.b = 9; t1.c=0; 都是错的 不能在类外使用

	system("pause");
	return;
}
//三看原则：看调用语句 是写在子类的内部还是外部  看是什么继承  看父类中 访问级别

void main2()
{
	Child2 t1, t2;
	//t1.a = 10;
	
	//t1.b = 9; t1.c=0; 都是错的 不能在类外使用

	system("pause");
	return;
}

void main()
{
	Child3 t1, t2;
	//t1.a = 10;

	//t1.b = 9; t1.c=0; 都是错的 不能在类外使用

	system("pause");
	return;
}

//