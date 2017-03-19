#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


//1.值方式
void SwapByValue(int x,int y)
{
	int temp = x;
	x = y;
	y = temp;
}

//2. 地址传递
void SwapByPointer(int* x,int* y)
{
	int temp = *x;
	*x = *y;
	*y = temp;
}

//3. 引用传递 引用做参数不需要初始化
void SwapByReference(int& x,int& y){
	int temp = x;
	x = y;
	y = temp;
}


void test01(){
	
	int a = 10;
	int b = 20;

	//1. 值
	SwapByValue(a,b);
	cout << "a:" << a << endl;
	cout << "b:" << b << endl;
	cout << "----------" << endl;
	//2. 指针
	SwapByPointer(&a, &b);
	cout << "a:" << a << endl;
	cout << "b:" << b << endl;
	cout << "----------" << endl;
	//3. 引用
	SwapByReference(a,b);
	cout << "a:" << a << endl;
	cout << "b:" << b << endl;
	cout << "----------" << endl;

}

//2. 引用做函数的返回值

int& DoBussiness(){
	//1. 不要返回局部变量的引用
	//int a = 10;
	static int a = 10;
	return a;
}

void test02(){

	//如果想函数的返回值当左值，那么就返回引用
	DoBussiness() = 100;
}


int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}