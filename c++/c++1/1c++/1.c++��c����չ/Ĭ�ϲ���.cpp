#include <iostream>
using namespace std;

void myPrint(int x = 3)
{
	cout << "x" << x << endl;
}

void myPrint2(int m,int n,int x = 3, int y = 4)//void myPrint(int x = 3,int y=4,int m)编译过不去
{											  //如果有默认参数一定要放于后面
	cout << "x" << x << endl;
}
void main1()
{
	myPrint(4);//若你填写参数 就会使用你填写的 不填写就默认
	myPrint();
	system("pause");
	return;
}
//函数占位参数
void func1(int a, int b, int )
{
	cout << "a" << a << "b" << b << endl;

}
void main2()
{
	//func1(1, 2);函数调用时必须写够参数
	func1(1, 2, 2);

	system("pause");
	return;
}
//默认参数和占位参数在一起
void func2(int a, int b, int = 0)
{
	cout << "a：" << a << "b ：" << b << endl;
}

void main()
{
	func2(1,2);//都没错
	func2(1, 2, 3);//都没错 3自吞了
	system("pause");
	return;
}