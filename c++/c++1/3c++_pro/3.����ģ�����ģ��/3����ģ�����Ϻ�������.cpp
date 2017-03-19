#include <iostream>
using namespace std;

template < typename T >
void myswap(T &a,T&b)
{
	T c = 0;
	c = a;
	a = b;
	b = c;
	cout << "我是模板函数" << endl;
}

void myswap(int a,char c)//重载了
{

	cout << "a:" << a << "c:" << c << endl;
	cout << "我是普通函数" << endl;
}


void main()
{
	int a = 10;
	char c = 'z';
	//调用函数模板 本质是类型参数化 将严格的按照类型进行匹配 不会进行自动类型转换，而普通函数调用编译器会自动的转换
	myswap(a, c);//调用第二个普通函数  因为调用函数模板要求实参形参类型得相同
	myswap(c, a);//还是第二个               函数模板要求严格的匹配
	myswap(a, a);//调函数模板                           


	system("pause");
	return;
}