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

	

	//测试赋值
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
		cout << "str1 和 str2 相同的字符串!" << endl;
	}

	if (str1 == "abcd"){
		cout << "str1 和 abcd 相同的字符串!" << endl;
	}

	if (str1 != str2){
		cout << "str1 和 str2 不相同的字符串!" << endl;
	}

	if (str1 != "abcd"){
		cout << "str1 和 abcd 不相同的字符串!" << endl;
	}

}

void test03() {
        mystring str1;
        str1 = "adcd"; //char* 参数构造                //为什么不调char*拷贝???
}

void test04() {
        mystring str1;
        str1 = mystring("adcd");  //赋值拷贝
}

void test05() {
        mystring str1 = mystring("adcd"); //char*参数构造  拷贝构造
}



int main(){

	test01();
	test02();

	//system("pause");
	//return EXIT_SUCCESS;
	return 0;
}
