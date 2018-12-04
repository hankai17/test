#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Animal{
public:
	virtual void func1(){
		cout << "我是基类的func1!" << endl;
	}
	virtual void func2(){
		cout << "我是基类的func2!" << endl;
	}
};

class Dog : public Animal{
public:
	virtual void func3(){
		cout << "我是子类的func3!" << endl;
	}
	virtual void func4(){
		cout << "我是子类的func4!" << endl;
	}
};


int main(){

	Animal* animal = new Dog;
	animal->func1();

	system("pause");
	return EXIT_SUCCESS;
}