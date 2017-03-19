#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//只能抛出const char*类型和char类型异常
void func1() throw(const char*,char) {
	throw 10;
}

void test01(){
	
	try{
		func1();
	}
	catch (int){
		cout << "捕获Int类型异常!" << endl;
	}
}

//什么类型异常都不能抛出
void func2() throw() {
	throw 10;
}

void test02(){
	
	try{
		func2();
	}
	catch (int){
		cout << "捕获Int类型异常!" << endl;
	}
}

//可以抛出任何类型的异常
void func3(){
	throw 10;
}

void test03(){
	try{
		func3();
	}
	catch (int){
		cout << "捕获Int类型异常!" << endl;
	}
}

int main(){

	//test01();
	//test02();
	test03();
	
	system("pause");
	return EXIT_SUCCESS;
}