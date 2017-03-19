#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"Person.hpp"

void test(){
	
	Person<int> person(10);
	person.PirntPerson();

	//和c++编译机制有关 分文件编译，一个cpp就是一个编译单元
	//二次编译有关

}

int main(){

	test();


	system("pause");
	return EXIT_SUCCESS;
}