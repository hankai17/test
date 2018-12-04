#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class MyFunction{
public:
	//函数调用符号
	int operator()(int v1,int v2){
		//cout << "hello world!" << endl;
		return v1 + v2;
	}
};

int main(){

	MyFunction myfunc;
	cout << myfunc(10,20) << endl; //operator()  函数对象  仿函数

	system("pause");
	return EXIT_SUCCESS;
}