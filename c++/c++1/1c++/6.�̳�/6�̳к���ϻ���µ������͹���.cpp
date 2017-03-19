#include <iostream>
using namespace std;
class Object
{
public:
	Object(int a, int b)
	{
		this->a = a;
		this->b = b;
		cout << "obect构造函数执行" << "a" << a << "b" << b << endl;
	}
	~Object()
	{
		cout << "Object析构函数" << endl;
	}
protected:
	int a;
	int b;
};

class Parent:public Object
{
public:
	Parent(char * p) :Object(1,2)
	{
		this->p = p;
		cout << "父类构造函数" << endl;
	}
	~Parent()
	{
		cout << "父类析构函数" << endl;
	}
	void printP(int a, int b)
	{
		cout << "我是爹" << endl;
	}

	
protected:
	char * p;
private:
	

};

class Child :public Parent
{
public:
	Child(char * p) :Parent(p),obj1(3,4),obj2(5,6)//构造函数的初始化列表
	{
		this->myp = p;
		cout << "子类构造函数"<<myp << endl;
	}
	~Child()
	{
		cout << "子类析构函数" <<myp<< endl;
	}

	void printC()
	{
		cout << "我是儿子" << endl;
	}
private:
protected:
	char * myp;
	Object obj1;
	Object obj2;
};


void playobj()
{
	Child c1("继承测试");
}
void main()
{
	playobj();//先调用老祖宗的构造函数 在调用老爹的构造函数 在调用组合对象obj1 obj2的构造函数 最后调用自己的构造函数
	system("pause");

}
//先调用老祖宗 老爹 object(组合对象的) 最后自己构造函数 析构则相反