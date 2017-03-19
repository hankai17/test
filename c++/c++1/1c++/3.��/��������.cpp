#include <iostream>
using namespace std;

class Cube
{
public:
	void setA(int a)
	{
		m_a = a;
	}
	void setB(int b)
	{
		m_b = b;
	}
	void setC(int c)
	{
		m_c = c;
	}
	void setABC(int a,int b,int c)
	{
		m_a = a; m_b = b; m_c = c;
	}
public:
	int getV()
	{
		m_v = m_a*m_b*m_c;
		return m_v;
	}
	int getS()
	{
		m_s = 2*(m_a*m_b + m_a*m_c + m_b*m_c);
		return m_s;
	}
public://只有通过public接口 访问（改变）属性！！！！！！！！！！！！！！！！！！
	int getA()
	{
		return m_a;
	}
	int getB()
	{
		return m_b;
	}
	int getC()
	{
		return m_c;
	}
public://用面向对象的方法           3000
	int judgeCube(Cube &v1, Cube &v2)
	{
		if ((v1.getA() == v2.getA()) &&
			(v1.getB() == v2.getB()) &&
			(v1.getC() == v2.getC()))
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}

public:                 //4000
	int judgeCube(Cube &v2)//重载 好多学员分不清 这个场景下的m_a是属于v1还是属于v2
	{
		if (m_a ==v2.getA()&&
			m_b ==v2.getB()&&
			m_c ==v2.getC())//v1的类的私有属性内部可以调用

		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
private:
	int m_a;
	int m_b;
	int m_c;
	int m_v;
	int m_s;
};

//全局函数法 判断两个立方体是否相等
int judgeCube(Cube &v1, Cube &v2)
{
	if ((v1.getA() == v2.getA()) &&
		(v1.getB() == v2.getB()) &&
		(v1.getC() == v2.getC()))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



void main1()//全局函数调用
{
	Cube v1, v2;
	v1.setABC(1, 2, 2);
	cout << v1.getS()<<endl;
	cout << v1.getV() << endl;

	if (judgeCube(v1, v2) == 1)
	{
		cout << "相等" << endl;
	}
	else
	{
		cout << "不相等" << endl;
	}
	
	system("pause");

}
void main()//全局函数调用
{
	Cube v1, v2;
	v1.setABC(1, 2, 2);
	cout << v1.getS() << endl;
	cout << v1.getV() << endl;

	v2.setABC(2, 2, 2);
	if (v1.judgeCube(v1, v2) == 1)//这句话 很奇怪有冗余 写成v1.judgeCube(v2)
	{							  //判断v1 v2立方体是否相等 通过类成员函数
		cout << "相等" << endl;
	}
	else
	{
		cout << "不相等" << endl;
	}

	int tag=v1.judgeCube(v2);//v1.judgeCube(&v2);!!!!!!!!!!!!!!!!!!!!!!!千万别这样写
	if (tag == 1)//这句话 很奇怪有冗余 写成v1.judgeCube(v2)
	{							  //判断v1 v2立方体是否相等 通过类成员函数
		cout << "相等" << endl;
	}
	else
	{
		cout << "不相等" << endl;
	}
	system("pause");

}

