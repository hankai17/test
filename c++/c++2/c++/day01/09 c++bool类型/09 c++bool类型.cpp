#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<iomanip>
using namespace std;

int main(){


	bool flag = true;
	cout << flag << endl;
	flag = 10;
	cout << flag << endl;
	flag = 0;
	cout << flag << endl;

	//cout << setiosflags(ios::boolalpha) << flag << endl;

	cout << "bool size:" << sizeof(bool) << endl;



	system("pause");
	return EXIT_SUCCESS;
}