#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

template<class NameType,class AgeType>
class Person{
public:
	Person(NameType name,AgeType age){
		this->mName = name;
		this->mAge = age;
	}
public:
	NameType mName;
	AgeType mAge;
};


//1. 
void DoBussiness(Person<string,int>& person){
	cout << "Name:" << person.mName << " Age:" << person.mAge << endl;
}

//2.函数模板自动推导
template<class T1,class T2>
void DoBussiness2(Person<T1, T2>& person){
	cout << "Name:" << person.mName << " Age:" << person.mAge << endl;
}

template<class T>
void fun(){
	cout << "hello world!" << endl;
}


int main(){

	int a = 10;
	fun<int>();

	Person<string, int> person("John", 30);
	DoBussiness(person);

	DoBussiness2(person);

	system("pause");
	return EXIT_SUCCESS;
}