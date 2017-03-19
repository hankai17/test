#include <iostream>
using namespace std;

void divide(int x, int y)
{
	if (y==0)
	{
		throw x;//抛出int型异常

	}
	cout << "结果："<<x / y << endl;
}

void myDivide(int x, int y)
{
	divide(x, y);

}


void main()
{
	try
	{
		divide(10, 2);
		divide(10, 0);
	}
	catch (int e)
	{
		cout << e << "被0除" << endl;
	}

	catch (...)
	{
		cout << "其他未知异常" << endl;
	}


	system("pause");
	return;
}
void main2()
{
	try
	{
		throw 'Z';
	}
	catch (int e)
	{
		cout << "捕获int型异常" << endl;
	}

	catch (...)
	{
		cout << "未知类型异常" << endl;
	}
	system("pause");
}
//没看懂