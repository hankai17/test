#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Person{
public:
	Person(string name){
		mName = name;
		cout << mName << "���󱻴���!" << endl;
	}
	~Person(){
		cout << mName << "��������!" << endl;
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
		cout << "�쳣����!" << endl;
	}



	system("pause");
	return EXIT_SUCCESS;
}