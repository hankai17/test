#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


//const_cast
void test01(){

	int* p = new int;

	//1. c风格
	const int* p1 = (const int*)p;
	//2. c++风格
	const int* p2 = const_cast<const int*>(p);


	int* p3 = (int*)p1;

	int* p4 = const_cast<int*>(p2);
}



int main(){



	system("pause");
	return EXIT_SUCCESS;
}