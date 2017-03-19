#include<iostream>
using namespace std;



void fun()
{
	const int a = 10;
	int *p = (int *)&a;
	
	*p = 100;

	cout << *p<< endl;
	cout << a << endl;
	
	return;
}




int main()
{
	fun();
	system("pause");
	return 0;
}


