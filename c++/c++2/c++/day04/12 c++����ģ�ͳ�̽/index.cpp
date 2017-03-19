#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class class1{
public:
	int mA;
	int mB;
};

class class2{
public:
	int mA;
	static int mB;
};

class class3{
public:
	void func(){}
public:
	int mA;
	static int mB;
};

class class4{
public:
	static void func1(){}
public:
	int mA;
	static int mB;
};

class class5{
public:
	void func(){
		cout << mA << endl;
	}
public:
	int mA;
};


struct class5{
	int mA;
};

void func(class5* pthis){
	cout << pthis->mA << endl;
}


void test01(){

	//c++里数据和函数是分开存储的
	cout << "class1 size:" << sizeof(class1) << endl;
	cout << "class2 size:" << sizeof(class2) << endl;
	cout << "class3 size:" << sizeof(class3) << endl;
	cout << "class4 size:" << sizeof(class4) << endl;
	cout << "class5 size:" << sizeof(class5) << endl;
}

//1. c++的函数和数据分开存储
//2. c++不会为每一个对象都会生成一段函数代码，只会生成一份
void test02(){

	class5 c1;// struct c1{ int mA; };
	class5 c2;// struct c2{ int mA; };

	c1.func();
	func(&c1)
}

int main(){


	system("pause");
	return EXIT_SUCCESS;
}