//子类的构造与析构都需要调用父类函数 先完成爹的初始化 再完成儿子的初始化
#include <iostream>
using namespace std;

class Parent
{
public:
	Parent(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout << "父类构造函数" << endl;
	}
	~Parent()
	{
		cout << "父类析构函数" << endl;
	}
	void printP(int a,int b)
	{
		this->a = a;
		this->b = b;
		cout << "我是爹" << endl;
	}
	
	Parent(const Parent & obj)
	{
		cout << "copy构造函数" << endl;
	}
private:
	int a;
	int b;

};

class Child :public Parent
{
public:
	Child(int a, int b, int c) :Parent(a,b)//构造函数的初始化列表
	{
		cout << "子类构造函数" << endl;
		this->c = c;
	}
	~Child()
	{
		cout << "子类析构函数" << endl;
	}

	void printC()
	{
		cout << "我是儿子" << endl;
	}
private:
	int c;
};


void playobj()
{
	Child c1(1, 2, 3);
}
void main()
{
	playobj();
	system("pause");

}
//先调用父类构造函数 再调用子类构造函数 析构则相反