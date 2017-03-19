#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//1. cout.flush
void test01(){
	cout << "hello world";
	cout.flush();
}

//2. cout.put 一次输出一个字符到缓冲区
void test02(){
	cout.put('h').put('e').put('l');
}

//3. cout.write 输出一个字符串
void test03(){
	
	//char* str = "hello world!";
	//cout.write(str, strlen(str) + 10);

	char* pStr = "************";
	for (int i = 1; i < strlen(pStr); i ++){
		cout.write(pStr, i);
		cout << endl;
	}

	for (int i = strlen(pStr); i > 0; i--){
		cout.write(pStr, i);
		cout << endl;
	}

}

int main(){

	//test01();
	//test02();
	test03();

	system("pause");
	return EXIT_SUCCESS;
}