#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


template<typename T>
void MyPlus(T a, T b){
	cout << a + b << endl;
}

template<typename T>
void MyPlus(T a, T b, T c){
	cout << a + b + c<< endl;
}

void MyPlus(int a, int b){
	cout << a + b << endl;
}


int main(){

	int a = 10;
	int b = 20;
	//1.如果函数模板和普通函数都能匹配调用，那么编译器优先考虑普通函数
	MyPlus(a, b);
	//2.
	MyPlus<>(a, b); //强制编译器调用函数模板

	//3. 如果函数模板能产生更好的匹配，那么就会调用函数模板
	char aa = 'a';
	char bb = 'b';
	MyPlus(aa, bb);

	//4. 函数模板也可以像普通函数那样重载
	MyPlus(10, 20, 30);


	system("pause");
	return EXIT_SUCCESS;
}