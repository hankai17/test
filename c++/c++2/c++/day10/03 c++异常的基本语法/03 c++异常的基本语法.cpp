#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

int divide(int x,int y){
	if (y == 0){
		throw 10; //抛出异常
	}


	//throw "abcdefg";

	throw 1.23;

	return x / y;
}


//没有处理异常
int divide2(int x, int y){

#if 0
	int ret = 0;
	try{
		ret = divide(x, y) + 10;
	}
	catch (int){
		cout << "除数为0!" << endl;
	}
#else
	int ret = divide(x, y) + 10;
#endif
	

	return ret;
}

void test01(){


	int ret = divide2(10, 10);
	//throw;

}

//1. 异常不能够被忽略，并且异常可以跨函数，异常并不是简单int数字，最好有明确语义
//2. c++提供异常机制，是跨函数，并且不可忽略
int main(){

	try{
		test01();
	}
	catch (int e){
		cout << "除数为" <<  e <<"!" << endl;
	}
	catch (const char* e){
		cout << "异常:" << e << endl;
	}
	catch (...){
		cout << "其他异常！" << endl;
	}

	system("pause");
	return EXIT_SUCCESS;
}