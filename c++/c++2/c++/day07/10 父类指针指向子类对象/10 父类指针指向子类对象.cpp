#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Animal{
public:
	virtual void func(){
		cout << "hello Animal!" << endl;
	}
	virtual void func1(){
		cout << "hello func1!" << endl;
	}
};

class Dog : public Animal{
public:
	virtual void func(){
		cout << "hello Dog!" << endl;
	}
	virtual void func1(){
		cout << "hello func1!" << endl;
	}
};

int main(){

	Animal* animal = new Dog;
	animal->func();

	animal->func1();

	system("pause");
	return EXIT_SUCCESS;
}