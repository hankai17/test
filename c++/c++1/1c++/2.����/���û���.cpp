#include <iostream>
using namespace std;

void main01()
{
	int a = 10;
	int &b = a;//普通引用一定要出事化 要给它一个值
	printf("a的地址： %d\n", &a);
	printf("b的地址： %d\n", &b);//结果地址都一样
	system("pause");
	return;
}

void  myswap(int * a, int * b)
{
	int t;
	t = *a;
	*a = *b;
	*b = t;
	return;
}

void myswap1(int &a, int &b)
{
	int t;
	t = a;
	a = b;
	b = t;
}
void main()
{
	int x = 20, y = 30;
	printf("x:%d y:%d\n", x, y);
	
	myswap(&x, &y);
	printf("x:%d y:%d\n", x, y);

	x = 20; y = 30;
	myswap1(x, y);
	printf("x:%d y:%d\n", x, y);

	system("pause");
	return;

}

