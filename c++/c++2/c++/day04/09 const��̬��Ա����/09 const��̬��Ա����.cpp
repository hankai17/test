#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class MyClass{
public:
	static void ShowMyClass(){
		//cout << mB << endl;
	}

	void PrintMyClass(){
		cout << a << endl;
		cout << mB << endl;
	}
public:
	const static int a = 0; //const静态成员变量
	int mB;
};


int main(){


	cout << MyClass::a << endl;

	system("pause");
	return EXIT_SUCCESS;
}