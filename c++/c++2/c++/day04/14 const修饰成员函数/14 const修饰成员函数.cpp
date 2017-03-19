#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Person{
public:
	Person(){
		mID = 0;
		mAge = 0;
 	}

	//this指针 const Person* const this;
	//成员函数后加上const表示，我只会去使用对象的数据，不会去修改对象的数据
	void PrintPerson() const{
		//this->mID = 10;
		//this->mAge = 20;
	}

public:
	//mutable 易于变动的 容易改变
	mutable int mID;
	int mAge;
};


//const修饰对象
void test01(){

	const Person person;
	//常对象不能修改普通的成员变量，但是修改mutable修饰的成员变量
	//person.mAge = 100;
	person.mID = 100;

	//常对象只能调用const修饰的函数
	person.PrintPerson();


}
int main(){



	system("pause");
	return EXIT_SUCCESS;
}