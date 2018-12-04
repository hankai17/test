#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Myclass{
public:
	Myclass(){
	
	}
	//1. 如果提供拷贝构造函数，那么编译器不会给你提供任何的默认构造函数
	Myclass(const Myclass& mclass){
	
	}
};

class Myclass2{
public:
	Myclass2(){

	}

	//提供默认的拷贝构造
};

int main(){

	Myclass mclass1;
	Myclass mlcass2 = mclass1;

	Myclass2 mclass3;
	Myclass2 mclass4 = mclass3;

	system("pause");
	return EXIT_SUCCESS;
}