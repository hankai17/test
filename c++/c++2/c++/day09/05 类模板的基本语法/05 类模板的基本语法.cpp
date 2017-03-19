#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

template<class T>
class MyClass{
private:
	T* pAddress; //指向一个数组空间
};

int main(){

	//使用类模板
	MyClass<int> mclass; //必须显示指定类型，不能自动类型推导

	system("pause");
	return EXIT_SUCCESS;
}