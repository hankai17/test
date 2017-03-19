#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


class Complex{
public:
	Complex(){
		mID = 0;
	}
	Complex& operator++(){
		++this->mID;
		return *this;
	}

	Complex operator++(int){
		Complex complex = *this;
		++this->mID;
		return complex;
	}

	void ShowConplex(){
		cout << "ID:" << mID << endl;
	}
private:
	int mID;
};



int main(){


	Complex complex;
	++complex;
	complex.ShowConplex(); //1


	Complex com =  complex++;
	com.ShowConplex(); //2


	system("pause");
	return EXIT_SUCCESS;
}