#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

template<class T>
class Base{
public:
	T mA;
};


template<class T>
class Derived : public Base<T>{
public:
	T mB;
};

//关于类模板的继承，关键点，在于继承的时候，要确定基类的大小


int main(){

	Derived<int> d;


	system("pause");
	return EXIT_SUCCESS;
}