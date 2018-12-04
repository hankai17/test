#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include"MyArray.h"
using namespace std;

//1. []运算符重载
void test01(){

	//创建数组
	MyArray marray;
	
	for (int i = 0; i < 10; i++){
		//调用重载[]运算符函数
		marray[i] = i + 1;
	}

	for (int i = 0; i < 10;i++){
		cout << marray[i] << " ";
	}

}
//2. +运算符重载
class OtherClass{
public:
	OtherClass(int param){
		mParam = param;
	}
public:
	int mParam;
};
class MyClass{
	
public:
	MyClass(int param1,int param2){
		mParam1 = param1;
		mParam2 = param2;
	}
	void ShowMyClass(){
		cout << "mParam1:" << mParam1 << " mParam2:" << mParam2 << endl;
	}
	//成员函数操作符重载
	MyClass operator+(const MyClass& mclass){
		int newParam1 = this->mParam1 + mclass.mParam1;
		int newParam2 = this->mParam2 + mclass.mParam2;
		cout << "成员！" << endl;
		MyClass temp(newParam1, newParam2);
		return temp;
	}
	//成员函数操作符重载
	MyClass operator+(const OtherClass& mclass){
		int newParam1 = this->mParam1 + mclass.mParam;
		int newParam2 = this->mParam2 + mclass.mParam;
		cout << "成员！" << endl;
		MyClass temp(newParam1, newParam2);
		return temp;
	}
public:
	int mParam1;
	int mParam2;
};

//全局运算符重载
MyClass operator+(const MyClass& class1, const MyClass& class2){
	int newParam1 = class1.mParam1 + class2.mParam1;
	int newParam2 = class2.mParam2 + class2.mParam2;
	cout << "全局！" << endl;
	MyClass temp(newParam1, newParam2);
	return temp;
}

void test02(){
	
	MyClass class1(10, 10);
	MyClass class2(20, 20);
	MyClass mclass = class1 + class2; //class1.operator+(class2);
	

	//举例2. 不同的两个对象之间相加
	OtherClass oclass(10);
	MyClass classret = class1 + oclass; // class1.operator+(oclass)
	classret.ShowMyClass();
#if 0
	//报错: 解决方案是在OtherClass里也要重载+号操作符
	MyClass classret = oclass + class1; //oclass.operator+(class1)
	classret.ShowMyClass();
#endif

}


int main(){

	//test01();
	test02();

	system("pause");
	return EXIT_SUCCESS;
}