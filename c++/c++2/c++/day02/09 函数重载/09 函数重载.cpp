#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

void func(){
	cout << "hello world!" << endl;
}
void func(int x){
	cout << x << endl;
}

void func(double x){
	cout << x << endl;
}

void func(int x,double y = 0){
	cout << "x:" << x << " y:" << y << endl;
}

void func(double x, int y){
	cout << "x:" << x << " y:" << y << endl;
}

//函数的返回值不能作为重载的条件
#if 0
int func(double x, int y){
	return x + y;
}
#endif

void test01(){

	int a = 10;
	double b = 3.14;
	//func(a);
	func(a, b);

	//函数调用的时候，是根据参数类型匹配
}

//为什么返回值不能作为重载的条件
void test02(){
	
	//int ret = func(b, a);
	//func(b, a);
}

//函数重载碰到参数默认值
void test03(){
	//func(10); //函数重载碰到默认参数，那么要考虑是否会出现函数调用二义性
}

namespace A{
	void func(char c){
		cout << "void func(char c) " << endl;
	}
#if 0
	void func(int x){
		cout << "void func(int x)" << endl;
	}
#endif
}

void test04(){
	char c = 'a';
	int x = 10;
	A::func(c);
	A::func(x);
}

int main(){

	//test01();
	test04();

	system("pause");
	return EXIT_SUCCESS;
}