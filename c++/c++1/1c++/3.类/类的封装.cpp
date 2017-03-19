//封装：1.把属性和方法封装2.对属性和方法进行访问控制
//public 可以在类的内部和外部使用 private只能在类的内部访问 protected只能在累的内部访问只用在继承里面
#include <iostream>
using namespace std;

class MyCircle
{
public :
	double m_r;//属性也叫成员变量
	double m_s;
public:
	double getR()
	{
		return m_r;
	}
	void setR(double r)//成员函数
	{
		m_r = r;
	}
	double getS()
	{
		m_s = 3.14*m_r*m_r;
		return m_s;
	}
protected:
private:

};

void printCircle01(MyCircle *pC)//类封装了成员变量和函数  两者都可以直接调用（封装的威力）！！！！！
{
	cout << "r:" << pC->getR()<< endl;
	cout << "s:" << pC->getS()<< endl;
}

void printCircle02(MyCircle &myc)
{
	cout << "r:" << myc.getR() << endl;
	cout << "s:" << myc.getS() << endl;
}

void main1()
{
	MyCircle c1, c2;
	c1.setR(10);
    printCircle01(&c1);
	c2.setR(11);
	printCircle02(c2);
	system("pause");
}

class MyAdvCircle
{
private:
	double m_r;//属性也叫成员变量
	double m_s;
public:
	double getR()
	{
		return m_r;
	}
	void setR(double r)//成员函数
	{
		m_r = r;
	}
	double getS()
	{
		m_s = 3.14*m_r*m_r;
		return m_s;
	}
};

void main2()
{
	MyAdvCircle c1, c2;
	//c1.m_r = 10;//不能访问私有成员 //类的私有成员相当于银行卡密码
	c1.setR(10);//只能通过函数       //相当于姓名
	system("pause");
}

struct AAA
{
	int a;//用struct定义的类 不写 默认属性是public而class正好相反！！！！！！！！！！
};

void main()
{
	AAA a1;
	a1.a = 11;
	system("pause");
}