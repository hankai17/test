#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

int main(){

	//1. 创建对组
	pair<int, int> p(10, 20);
	cout << p.first << endl;
	cout << p.second << endl;

	pair<string, int> p2 = make_pair("hello", 10);
	cout << p2.first << endl;
	cout << p2.second << endl;


	system("pause");
	return EXIT_SUCCESS;
}