#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Person{
public:
	Person(){
		mID = 10;
	}
	Person(int id){
		mID = id;
	}
public:
	int mID;
};


//区别
void test01(){
	Person p1;
	Person p2 = p1; //拷贝构造函数
	//由于p2还没有创建，所以p2应该调用构造函数

	Person p3; //p3创建好了
	p3 = p1; //赋值操作
}

//如果你没有写operator=函数，编译器会给你提供一个默认赋值操作函数
void test02(){

	Person p1(100);
	Person p2; 
	p2 = p1;

	cout << "p1:" << p1.mID << endl;
	cout << "p2:" << p2.mID << endl;
}

class People{
public:
	People(){
		mID = 10;
		this->pName = new char[strlen("aaa") + 1];
		strcpy(this->pName, "aaa");
	}
	People(const char* name ,int id){
		this->pName = new char[strlen(name) + 1];
		strcpy(this->pName, name);
		mID = id;
	}

	People(const People& people){
		this->pName = new char[strlen(people.pName) + 1];
		strcpy(this->pName, people.pName);
		mID = people.mID;
	}

	People& operator=(const People& people){

		if (this->pName != NULL){
			delete[] this->pName;
			this->pName = NULL;
		}

		this->pName = new char[strlen(people.pName) + 1];
		strcpy(this->pName, people.pName);
		mID = people.mID;

		return *this;
	}

	~People(){
		if (this->pName != NULL){
			delete[] this->pName;
			this->pName = NULL;
		}
	}
public:
	int mID;
	char* pName;
};


void test03(){

	People p1("John",10);
	People p2;
	p2 = p1;
	p2 = p1;
}

int main(){

	//test02();
	test03();

	system("pause");
	return EXIT_SUCCESS;
}