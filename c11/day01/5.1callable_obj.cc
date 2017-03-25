#include <iostream>

void func(void)
{
	//...
}

struct Foo
{
	void operator()(void)
	{
		//...
	}
};

struct Bar
{
	using fr_t = void(*)(void);

	static void func(void)
	{
		//...
	}

	operator fr_t(void)
	{
		return func;
	}
};

struct A
{
	int a_;

	void mem_func(void)
	{
		//...
	}
};

int main(void)
{
	void(*func_ptr)(void) = &func;  //函数指针
	func_ptr();

	Foo foo;  //仿函数
	foo();

	Bar bar;  //可被转换为函数指针的类对象
	bar();

	void(A::*mem_func_ptr)(void) = &A::mem_func;  //类成员函数指针
	int A::*mem_obj_ptr = &A::a_;  //类成员指针

	A aa;
	(aa.*mem_func_ptr)();
	aa.*mem_obj_ptr = 123;

	system("pause");
	return 0;
}
//可调用对象有：
//函数指针 
//仿函数即有operator()成员函数的类对象 
//一个可转化为函数指针的类对象 
//类成员函数/指针
//所有的调用方法基本上都是在后面加()
