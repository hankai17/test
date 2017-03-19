#pragma once

#include <iostream>
using namespace std;

template <typename T>
class Complex
{
	//friend Complex MySub(Complex &c1, Complex &c2);


	friend ostream & operator << <T>(ostream &out, Complex &c3);//我这个不是bug！！！！

public:
	Complex(T a, T b);
	void printCom();
	Complex operator +(Complex &c2);
	Complex operator -(Complex &c2);
protected:
private:
	T a;
	T b;

};