#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Aclass{
public:
	Aclass(int a){
		cout << "A 构造函数!" << endl;
	}
	~Aclass(){
		cout << "A 析构函数!" << endl;
	}
	int mA;
};

class Bclass : public Aclass{
public:
	Bclass() : Aclass(10){
		cout << "B 构造函数!" << endl;
	}
	~Bclass(){
		cout << "B 析构函数!" << endl;
	}
	int mB;
};

class Cclass : public Bclass{
public:
	Cclass(){
		cout << "C 构造函数!" << endl;
	}
	~Cclass(){
		cout << "C 析构函数!" << endl;
	}
	int mC;
};


void test01(){

	cout << "A size:" << sizeof(Aclass) << endl;  //4
	cout << "B size:" << sizeof(Bclass) << endl;  //8
	cout << "C size:" << sizeof(Cclass) << endl;  //12
}

void test02(){

	Bclass b;
}

int main(){

	test02();


	system("pause");
	return EXIT_SUCCESS;
}