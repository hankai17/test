#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

#include"MyArray.hpp"

//1. 测试基础数据类型
void test01(){
	
	MyArray<int> arr(10);

	//数组中插入元素
	for (int i = 0; i < 10; i ++){
		arr.PushBack(i);
	}
	//打印数组中的元素
	for (int i = 0; i < 10; i ++){
		cout << arr[i] << " ";
	}
	cout << endl;

	cout << "-------------------" << endl;
	MyArray<int> arr2(arr);
	//打印数组中的元素
	for (int i = 0; i < 10; i++){
		cout << arr2[i] << " ";
	}
	cout << endl;

}

//2. 自定义数据类型

class Person{
public:
	Person(){
		this->pName = new char[strlen("undefined") +1];
		strcpy(this->pName, "undefined");
		this->mAge = -1;
	}
	Person(const char* name,int age){
		this->pName = new char[strlen(name) + 1];
		strcpy(this->pName, name);
		this->mAge = age;
	}

	//拷贝构造
	Person(const Person& person){

		this->pName = new char[strlen(person.pName) + 1];
		strcpy(this->pName, person.pName);
		this->mAge = person.mAge;

	}

	Person& operator=(const Person& person){

		if (this->pName != NULL){
			delete[] this->pName;
			this->pName = NULL;
		}

		this->pName = new char[strlen(person.pName) + 1];
		strcpy(this->pName, person.pName);
		this->mAge = person.mAge;

		return *this;
	}

	~Person(){
		if (this->pName != NULL){
			delete[] this->pName;
			this->pName = NULL;
		}
	}
public:
	char* pName;
	int mAge;
};

void test02(){


	//1.自定义类必须提供默认构造函数

	MyArray<Person> arr(10);

	Person p1("name1", 10);
	Person p2("name2", 20);
	Person p3("name3", 30);
	Person p4("name4", 40);
	Person p5("name5", 50);

	arr.PushBack(p1);
	arr.PushBack(p2);
	arr.PushBack(p3);
	arr.PushBack(p4);
	arr.PushBack(p5);

	//打印对象
	for (int i = 0; i < 5; i ++){
		cout << "Name:" << arr[i].pName << " Age:" << arr[i].mAge << endl;
	}


}



int main(){

	//test01();
	test02();

	system("pause");
	return EXIT_SUCCESS;
}