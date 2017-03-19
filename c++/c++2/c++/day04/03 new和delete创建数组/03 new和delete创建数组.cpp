#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//new基础数据类型数组
void test01(){
	int* pInt = new int[20];
	char* str = new char[20];

	delete[] pInt;
	delete[] str;
}

class Person{
public:
	Person(){
		cout << "无参构造函数！" << endl;
		mName = "undefined!";
		mAge = 0;
	}
	Person(string name, int age){
		cout << "2个参构造函数！" << endl;
		mName = name;
		mAge = age;
	}

	~Person(){
		cout << "析构函数！" << endl;
	}
public:
	string mName;
	int mAge;
};

void test02(){

	//创建自定义对象数组，必须提供无参构造函数
	Person* personArr = new Person[10];
	delete personArr;
}




int main(){

	test02();



	system("pause");
	return EXIT_SUCCESS;
}