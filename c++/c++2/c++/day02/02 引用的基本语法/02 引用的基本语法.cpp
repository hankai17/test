#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//引用基本语法
void test01(){

	int a = 10;
	int& b = a;
	b = 100;
	cout << "a:" << a << endl;
	cout << "b:" << b << endl;

	//只能给内存块取一个别名吗？n个别名
	int& c = a;
	c = 200;
	cout << "-------------" << endl;
	cout << "a:" << a << endl;
	cout << "b:" << b << endl;
	cout << "c:" << c << endl;

	cout << "-------------" << endl;
	cout << "a:" << &a << endl;
	cout << "b:" << &b << endl;
	cout << "c:" << &c << endl;
}

//引用的注意点
void test02(){
	
	//1. 引用必须初始化,必须和合法的内存块关联
	int b = 10;
	int& a = b; //引用一旦初始化，不能改变

	int c = 20; 
	a = c; //赋值行为，不是改变a的引用


	cout << "a:" << a << endl;
	cout << "b:" << b << endl;
	cout << "c:" << c << endl;

	//2. 不能对数组进行引用
	int arr[10];
	//int& arr[10] = arr;
}

void test03(){
	//1. 建立数组引用方法一
	typedef int ArrRef[10];
	int arr[10];
	ArrRef& aRef = arr;
	for (int i = 0; i < 10;i ++){
		aRef[i] = i+1;
	}
	for (int i = 0; i < 10;i++){
		cout << arr[i] << " ";
	}
	cout << endl;
	//2. 建立数组引用方法二
	int(&f)[10] = arr;
	for (int i = 0; i < 10; i++){
		f[i] = i+10;
	}
	for (int i = 0; i < 10; i++){
		cout << arr[i] << " ";
	}
	cout << endl;
}

int main(){

	//test01();
	//test02();
	test03();

	system("pause");
	return EXIT_SUCCESS;
}