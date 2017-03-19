#include <iostream>
using namespace std;
//函数的业务逻辑一样 但函数参数类型不一样
//有一种机制 让类型参数化 方便编程
//就是泛型编程（模板机制）
void myswap01(int &a,int &b)
{
	int c = 0;
	c = a;
	a = b;
	b = c;
}

void myswap02(char &a, char &b)
{  
	char c = 0;
	c = a;
	a = b;
	b = c;
}

template<typename T>//告诉编译器要进行泛型编程了 看见T不要报错
void myswap(T &a,T &b)
{
	T c;
	c = a;
	a = b;
	b = c;
	cout << "我是模板函数" << endl;
}

void main01()
{
	int x = 10;
	int y = 20;
	myswap01(x, y);
	printf("x:%d y%d \n", x, y);
	
	char a = 'a';
	char b = 'b';
	myswap02(a, b);
	printf("a:%c b:%c", a, b);
	system("pause");
	return;
}

//函数模板调用
//显示类型调用
//自动类型 推到
void main()
{
	int x = 10;
	int y = 20;
	myswap<int>(x, y);//显示调用 int是要参数化的类型
	printf("x:%d y%d \n", x, y);

	char a = 'a';
    char b = 'b';
	myswap<char>(a, b);//显示调用 int是要参数化的类型
	printf("x:%c y%c \n", a, b);

	system("pause");
	return;
}