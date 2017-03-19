#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class A{
public:
	static void func(){
		cout << "Hello world!" << endl;
	}
	static void func(int x){
		cout << "X:" << x << endl;
	}
public:
	static int mA;
};

int A::mA = 0;


class B : public A{
public:

	static int func(int x){
		return 0;
	}

	void testB(){
		func();
	}
};

int main(){

	B b;
	b.func();

	system("pause");
	return EXIT_SUCCESS;
}