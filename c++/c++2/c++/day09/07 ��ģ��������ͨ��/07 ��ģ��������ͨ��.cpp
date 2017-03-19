#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

template<class T>
class Base{
public:
	T mA;
};


//继承的话，应该继承一个具体的类，因为一个具体的类，编译器知道怎么分配空间
//继承不能继承一个类模板，因为编译器不知道为这个类模板分配多少内存
class Derived : public Base<int>{
public:
	int mB;
};

int main(){



	system("pause");
	return EXIT_SUCCESS;
}