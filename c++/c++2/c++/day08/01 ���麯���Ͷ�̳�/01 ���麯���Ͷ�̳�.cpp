#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Interface1{
public:
	virtual void func() = 0;
};

class Interface2{
public:
	virtual void func() = 0;
};

class Test : public Interface1, public Interface2{
public:
	virtual void func(){}
};



int main(){



	system("pause");
	return EXIT_SUCCESS;
}