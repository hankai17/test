#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"mystring.h"

void test01(){

	mystring str1("john ");
	mystring str2("Smith");

	mystring str3 = str1 + str2;

	mystring str5 = str1 + "hello world!";
	cout << "str5:" << str5 << endl;

	cout << "str3:" << str3 << endl;
	cout << "str3 size:" << str3.size() << endl;

	

	//���Ը�ֵ
	mystring str4;
	str4 = str3;
	cout << "str4:" << str4 << endl;
	str4 = "hello world";
	cout << "str4:" << str4 << endl;


}

void test02(){
	
	mystring str1 = "abcd";
	mystring str2 = "abcd";
	if (str1 == str2){
		cout << "str1 �� str2 ��ͬ���ַ���!" << endl;
	}

	if (str1 == "abcd"){
		cout << "str1 �� abcd ��ͬ���ַ���!" << endl;
	}

	if (str1 != str2){
		cout << "str1 �� str2 ����ͬ���ַ���!" << endl;
	}

	if (str1 != "abcd"){
		cout << "str1 �� abcd ����ͬ���ַ���!" << endl;
	}

}

int main(){

	test01();
	test02();

	//system("pause");
	//return EXIT_SUCCESS;
	return 0;
}
