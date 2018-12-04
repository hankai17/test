#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class MyClass{
	//3. 静态成员也是有访问权限的
public:
	static void ShowMyClass(){
		//1. 静态成员函数不能访问非静态成员变量
		//cout << mB << endl;
	}

	void PrintMyClass(){
		//2. 非静态成员函数可以访问静态成员变量和非静态成员变量
		cout << a << endl;
		cout << mB << endl;
	}
public:
	//类内声明，类外初始化
	static int a; //静态成员变量
	int mB;
};


int main(){

	//1. 类名调用
	MyClass::ShowMyClass();

	//2. 对象名
	MyClass m1;
	m1.ShowMyClass();


	system("pause");
	return EXIT_SUCCESS;
}