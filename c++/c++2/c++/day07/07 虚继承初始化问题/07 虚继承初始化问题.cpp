#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class BigBase{
public:
	BigBase(int id,int age){
		cout << "BigBase 构造函数!" << endl;
		this->mID = id;
		this->mAge = age;
	}
public:
	int mID;
	int mAge;
};

class Base1 : virtual public BigBase{
public:
	Base1() : BigBase(10,10){
		cout << "Base1 构造函数!" << endl;
	}
};

class Base2 : virtual public BigBase{
public:
	Base2() : BigBase(20, 20){
		cout << "Base2 构造函数!" << endl;
	}
};

class Derived : public Base1, public Base2{
public:
	Derived() : BigBase(30, 30){
		cout << "Derived 构造函数!" << endl;
	}
};

int main(){

	//Base1 b1;
	//Base2 b2;
	//Derived d;

	system("pause");
	return EXIT_SUCCESS;
}