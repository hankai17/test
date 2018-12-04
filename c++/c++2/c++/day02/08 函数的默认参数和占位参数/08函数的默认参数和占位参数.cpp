#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

int func(int x = 0,int y = 0){
	return x + y;
}

void test01(){

	cout << func() << endl;
	cout << func(10) << endl;
	cout << func(10, 20) << endl;
}

//注意点: 1. 如果第二个位置参数设置了默认值，那么之后所有位置的参数都必须有默认值
int func1(int x,int y = 0,int z = 0){
	return x + y + z;
}

void test02(){

	cout << func1(10) << endl;
	cout << func1(10,20) << endl;
	cout << func1(10, 20,30) << endl;
}

//注意点 2 ：函数声明和函数的实现不能同时写默认参数
void func2(int x = 0,int y = 0);
void func2(int x, int y){}


//函数的占位参数
int func3(int x,int){
	return x;
}

int func4(int x, int = 0){
	return x;
}

void test03(){
	//函数的占位参数也是参数，必须也要给个值，只不过函数内部用不了而已
	cout << func3(10, 0) << endl;
	cout << func4(10) << endl;
}


int main(){

	//test01();
	test03();

	system("pause");
	return EXIT_SUCCESS;
}