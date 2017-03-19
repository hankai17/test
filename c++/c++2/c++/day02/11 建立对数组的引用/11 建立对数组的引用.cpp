#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

int main(){


	//1. 第一种方式 typedef
	typedef int MyArr[10];
	int arr[10];
	MyArr& mRef = arr;
	for (int i = 0; i < 10; i++){
		mRef[i] = i + 1;
	}
	for (int i = 0; i < 10;i++){
		cout << arr[i] << " ";
	}
	cout << endl;

	//2. 
	int(&arrRef)[10] = arr;
	for (int i = 0; i < 10; i++){
		arrRef[i] = i + 10;
	}
	for (int i = 0; i < 10; i++){
		cout << arr[i] << " ";
	}
	cout << endl;



	system("pause");
	return EXIT_SUCCESS;
}