#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Animal{
public:
	virtual void ShowAnimal() {};
	virtual ~Animal() = 0; //纯虚析构函数
};

Animal::~Animal(){}

class Dog : public Animal{
public:
	Dog(){
		pName = new char[strlen("Ahuang") + 1];
		strcpy(pName, "John");
	}
	virtual void ShowAnimal(){
		cout << "Name:" << pName << endl;
	}
	~Dog(){
		cout << "Dog 析构函数!" << endl;
		if (this->pName != NULL){
			delete[] this->pName;
			this->pName = NULL;
		}
	}
private:
	char* pName;
};

void test(){
	
	//1. 继承
	//2. 重写父类的虚函数
	//3. 父类的指针或者引用指向子类对象

	Animal* animal = new Dog;
	animal->ShowAnimal();
	delete animal;
}

int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}