#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


//1.int(*CallBack)(int,int)
//2.typedef
//typedef int(CallBack)(int, int);
typedef int(*CallBack)(int, int);

int MyPlus(int a,int b){
	return a + b;
}

int DoBussiness(int param1, int param2, CallBack callback){
	return callback(param1, param2);
}

int main(){

	DoBussiness(10, 20, MyPlus);
	//cout << DoBussiness(10, 20, MyPlus) << endl;

	CallBack callback = MyPlus;
	callback(10,20);
	(*callback)(10, 20);

	system("pause");
	return EXIT_SUCCESS;
}