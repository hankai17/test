
#include <iostream>
using namespace std;
#include "9complex.cpp"//否则找不到函数体


void main()
{
	Complex<int> c1(1, 2);
	Complex<int> c2(3, 4);

	Complex<int>c3 = c1 + c2;



	system("pause");
	return;
}
