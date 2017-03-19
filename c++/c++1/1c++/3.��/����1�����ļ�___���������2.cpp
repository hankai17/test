#include <iostream>
using namespace std;

//class MyPoint;//类的前置声明 告诉编译器在后面//还不行！！！！！！！！！！！！！！！1

class MyPoint
{
public:
	void setPoint(int _x1, int _y1)
	{
		x1 = _x1;
		y1 = _y1;
	}
	int getX1()
	{
		return x1;
	}
	int getY1()
	{
		return y1;
	}
private:
	int x1;
	int y1;
};
class AdCircle
{
public:
	void setCircle(int _r, int _x0, int _y0)
	{
		r = _r; x0 = _x0; y0 = _y0;
	}
private:
	int r;
	int x0;
	int y0;
public:
	int judge(MyPoint &myp)
	{
		int dd = (myp.getX1() - x0)*(myp.getX1() - x0) + (myp.getY1() - y0)*(myp.getY1() - y0);
		if (dd <= r*r)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
};


void main()
{
	AdCircle c1;
	MyPoint p1;

	c1.setCircle(5, 0, 0);
	p1.setPoint(2, 2);

	int tag=c1.judge(p1);//在圆内为1//调用类的成员函数目的就是对其属性进行比对
	if (tag==1)
	{
		cout << "在园内" << endl;
	}
	else
	{
		cout << "在园外" << endl;
	}
	system("pause");
}