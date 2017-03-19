#include <iostream>
using namespace std;

class B
{
public:
	int b;
	B()
	{
		cout << "B构造函数" << endl;//去掉virtual 调用2次 加上则调用一次 因为用的时候只需要一次 所以要加上virtual
	}

protected:
private:

};

class B1 :virtual public B
{
public:
	int b1;
protected:
private:
};

class B2 :virtual public B
{
public:
	int b2;
protected:
private:
};

class C :public B1, public B2
{
public:
	int c;

protected:
private:
};
void main1()
{
	C c1;
	c1.b1 = 100;
	c1.b2 = 99;
	c1.c = 9;
	//c1.b = 0; 是从b1取b还是从b2取b 由此出现虚继承 virtual
	//c1.b = 0;


	system("pause");
	return;
}

//实际中 不需要多继承
//虚继承 只解决共同老祖先的问题
