#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Person{
public:
	Person(string name,int age){
		this->name = name;
		this->age = age;
	}
	void func1(){
		cout << "hello world!" << endl;
	}
	void PrintPerson(){
		cout << "Name:" << this->name << " Age:" << this->age << endl;
	}
public:
	string name;
	int age;
};

void test01(){
	Person person("John",30);
	person.PrintPerson();
}


void test02(){
	
	Person* person = NULL;
	//person->func1();
	person->PrintPerson();

}


int main(){

	//test01();
	test02();


	system("pause");
	return EXIT_SUCCESS;
}