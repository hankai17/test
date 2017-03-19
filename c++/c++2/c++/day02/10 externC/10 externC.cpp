#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

#include"a.h"

#if 0
	extern "C" void func();

	#if 0
	extern "C" void func();
	#endif

	#ifdef __cplusplus
		extern "C"{  //告诉编译器按照c语言的方式去链接
			//void func();
	#include"a.h"
		}
	#endif

#endif


int main(){

	func(); 

	system("pause");
	return EXIT_SUCCESS;
}