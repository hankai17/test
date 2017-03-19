#include <iostream>
using namespace std;

struct Teacher
{
	char name[32];
	int age;
};

void printT1(Teacher *pT)
{
	cout << pT->age << endl;
}

void printT2(Teacher &pT)
{
	cout << pT.age << endl;
}

void printT3(Teacher pT)//与printT1一摸样
{
	pT.age = 100;
	cout << pT.age << endl;
}

void main()
{
	Teacher t1;
	t1.age = 30;
	printT1(&t1);//而不是printT(Teacher *t1)!!!!!!!!!!!!

	printT2(t1);

	printT3(t1);
	system("pause");
	return;

}

void myswap1(int &a, int &b)
{
	int t;
	t = a;
	a = b;
	b = t;
}