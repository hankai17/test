#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

int main(){

	int a = 10;
	int b = 20;

	printf("%d\n", a < b ? a : b);

	//请大家考虑个问题，三目运算符返回的到底是个什么？

	//(a < b ? a : b) = 100;  //a = 100

	//c和c++三目运算符区别
	//c语言返回是一个值，c++返回的是变量本身，所以c++可以对三目运算符返回的结果赋值

	system("pause");
	return EXIT_SUCCESS;
}