#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

void test01(){
	

	//c++const 才是真正的常量
	const int a = 10;
	int* pA = (int*)&a;
	*pA = 100;

	cout << "a:" << a << endl;
	cout << "*pA:" << *pA << endl;

	int arr[a];

}

//全局const常量放到只读数据段，受到只读数据段的保护
extern const int a;

//用变量初始化const常量
void test02(){

	int b = 10;
	const int a = b;
	const int c = 10;

	int* pA = (int*)&a;
	*pA = 100;

	cout << "a:" << a << endl;
	cout << "*pA:" << *pA << endl;
}

//对于结构体，自定义数据类型，const会分配内存
struct Person{
	Person(){
		mID = 10;
		mAge = 10;
	}
	int mID;
	int mAge;
};
void test03(){

	const Person person;
	cout << "ID:" << person.mID << " Age:" << person.mAge << endl;

	Person* pPerson = (Person*)&person;
	pPerson->mAge = 100;
	pPerson->mID = 100;

	cout << "ID:" << person.mID << " Age:" << person.mAge << endl;
}

int main(){

	//test02();
	test03();

#if 0
	cout << &a << endl;
	int* pA = (int*)&a;
	*pA = 100;
	//cout << "a:" << a << endl;



	//cout << "a:" << a << endl;
	//test01();


	const int a = 10; //#define a 10
	int *pA = (int*)&a;
	*pA = 100;
	cout << "a:" << a << "\nb:" << *pA << endl;

#endif

	system("pause");
	return EXIT_SUCCESS;
}