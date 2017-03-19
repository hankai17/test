//有没有一个属性 所有对象都有的 有 就是static定义的
//static提供了一个同类对象的共享机制

#include <iostream>
using namespace std;

class BB
{
public:
	void printC()
	{
		cout << "c:" << c << endl;
		
	}
	void AddC()
	{
		c = c + 1;
	}
	static void getC()//静态成员函数
	{
		cout << "c:" << c << endl;               //静态成员函数中 能调用普通成员函数 属性吗？ 不能普通成员变量函数。 只能使用静态的
		//cout << "a:" << a << endl;               不能普通变量！！！  不知道是哪个类的a
	}
private:
	int a;
	int b;
	static int c;//静态成员变量

};

int BB::c = 10;

void main()
{
	BB b1, b2, b3;
	b1.printC();
	b2.AddC();//11
	b3.printC();

	//静态成员函数调用方法
	b3.getC();//对象.
	BB::getC();//类明：：

	system("pause");
	return;
}

