#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Person{
public:
	Person(string name){
		mName = name;
		cout << mName << "对象被创建!" << endl;
	}
	~Person(){
		cout << mName << "对象被析构!" << endl;
	}
public:
	string mName;
};

void func(){
	
	Person p("aaa");

	throw 10;
}

int main(){


	try{
		func();
	}
	catch (int e){
		cout << "异常捕获!" << endl;
	}



	system("pause");
	return EXIT_SUCCESS;
}