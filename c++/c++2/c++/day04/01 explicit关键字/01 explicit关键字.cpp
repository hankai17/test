#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class MyString{
public:
	explicit MyString(int n){
	
	}

	MyString(const char* str){
	
	}
};

int main(){


	//MyString str1 = 10;
	MyString str3(10);
	MyString str2 = "abcd";


	system("pause");
	return EXIT_SUCCESS;
}