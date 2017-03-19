#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//1. 函数对象概念
class MyClass{
public:
	void operator()(){
		cout << "hello world！" << endl;
	}
};
void test01(){
	MyClass m;
	m();
	m.operator()();  //inline，函数对象就有可能被编译器内联编译
}

//有普通函数指针，为什么要用函数对象？
//1. 函数指针在运行阶段才能确定函数调用，函数对象可以在编译阶段就能确定函数调用
//2. 函数有类型吗？没有。但是函数对象有类型，可以模板的参数
//3. 函数对象超出普通函数的概念，函数对象可以有自己的状态
//4. 函数对象执行效率要高于普通的函数



int main(){
	


	system("pause");
	return EXIT_SUCCESS;
}