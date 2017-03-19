#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Animal{
public:
	 virtual void speak(){
		cout << "动物在说话!" << endl;
	}
};

class Dog : public Animal{
public:
	 virtual void speak(){
		cout << "汪星人在说话!" << endl;
	}
};

class Cat : public Animal{
public:
	virtual void speak(){
		cout << "苗仁仁在说话!" << endl;
	}
};


//不应该让它在编译时确定函数调用
void DoBussiness(Animal& animal){
	animal.speak();
}

void test01(){

	Dog dog;
	DoBussiness(dog);
	Cat cat;
	DoBussiness(cat);

}

void test02(){

	cout << "Animal size:" << sizeof(Animal) << endl;
	cout << "Dog size:" << sizeof(Dog) << endl;
	
}

//----------------
class Base{
public:
	virtual void func(){
		cout << "基类!" << endl;
	}
};

class Derived : public Base{
public:
	virtual void func1(){
		cout << "基类!" << endl;
	}
};

int main(){

	//test01();
	test02();

	Derived* d = new Derived;
	d->func();

	system("pause");
	return EXIT_SUCCESS;
}