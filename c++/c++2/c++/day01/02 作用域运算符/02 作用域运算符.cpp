#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

int a = 10; //全局

int main(){

	//全局变量a 隐藏，并不是覆盖
	int a = 20; //局部

	cout << a << endl;
	cout << ::a << endl;


	system("pause");
	return EXIT_SUCCESS;
}