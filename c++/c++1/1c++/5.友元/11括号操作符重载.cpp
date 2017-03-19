#include <iostream>
using namespace std;

class F
{
public:
	int operator()(int a, int b)
	{
		return a*a + b*b;
	}
private:

};

class F2
{
public:
	int Fun(int a, int b)
	{
		return a*a + b*b;
	}
private:

};

void main()
{
	F f;
	f(2, 4);//operator()(int a,int b)
	
	F2 f2;
	f2.Fun(2, 4);
	
	system("pause");
	return;
}