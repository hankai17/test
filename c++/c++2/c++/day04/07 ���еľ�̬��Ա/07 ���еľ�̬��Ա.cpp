#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class MyClass{
public:
	//类内声明，类外初始化
	static int a; //静态成员变量
	int mB;
};

//类外初始化
int MyClass::a = 10;

void test01(){

	MyClass m1;
	MyClass m2;

	//1. 通过类名访问
	cout << MyClass::a << endl;

	//2. 对象访问
	m1.a = 100;

	cout << "m1.a:" << m1.a << endl;
	cout << "m2.a:" << m2.a << endl;
	cout << MyClass::a << endl;
	
}


int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}