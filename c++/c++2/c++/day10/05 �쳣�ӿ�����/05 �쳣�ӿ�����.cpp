#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//ֻ���׳�const char*���ͺ�char�����쳣
void func1() throw(const char*,char) {
	throw 10;
}

void test01(){
	
	try{
		func1();
	}
	catch (int){
		cout << "����Int�����쳣!" << endl;
	}
}

//ʲô�����쳣�������׳�
void func2() throw() {
	throw 10;
}

void test02(){
	
	try{
		func2();
	}
	catch (int){
		cout << "����Int�����쳣!" << endl;
	}
}

//�����׳��κ����͵��쳣
void func3(){
	throw 10;
}

void test03(){
	try{
		func3();
	}
	catch (int){
		cout << "����Int�����쳣!" << endl;
	}
}

int main(){

	//test01();
	//test02();
	test03();
	
	system("pause");
	return EXIT_SUCCESS;
}