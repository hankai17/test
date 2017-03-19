#include <iostream>
using namespace std;


template <typename T>//告诉编译器要泛型编程了 出现T不要报错
class A
{
public:
	A(T a = 0)//A(int a = 0)
	{
		this->a = a;
	}
	void printA()
	{
		cout << "a:" << a << endl;
	}
protected:
	T a;
private:

};

class B :public A<int>//模板类派生 也需要具体化模板类  要知道父类占的内存大小是多少 只有数据类型固定下来才能分配内存
{
public:
	B(int a = 10, int b = 20) :A<int>(a)
	{
		this->b = b;
	}
	void printB()
	{
		cout << "a:" << a << " b:" << b << endl;
	}

private:
	int b;

};
template<typename T>
class C :public A<T>
{
public:
	C(T c, T a) :A<T>(a)
	{
		this->c = c;
	}
	void printC()
	{
		cout << "c:" << c << endl;
	}

protected:
	T c;

};


void main()//从模板类派生模板类
{
	C<int> c1(1, 2);
	c1.printC();
	system("pause");


}


void main62()
{
	B b1(1,2);
	b1.printB();
	system("pause");
}
//从模板类派生普通类
void UseA(A<int> &a)//形参也必须是具体的类 编译器才能分配内存
{
	a.printA();
}

void main61()
{
	
	//模板类是抽象的需要进行类型具体化 再由具体的类定义具体的对象//A a1(11);
	A<int>a1(11), a2(20), a3(30);
	a1.printA();
	UseA(a1);//类模板做函数参数

	system("pause");
	return;
}
