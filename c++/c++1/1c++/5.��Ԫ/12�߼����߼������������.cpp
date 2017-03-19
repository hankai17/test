//逻辑与或在C++中比较特殊有短路规则&&两边只要有一边为0就不执行了
//如果重载&&就没有了 短路规则
#include <iostream>
using namespace std;

class Test
{
	int i;
public:
	Test(int i)
	{
		this->i = i;
	}
	Test operator+(const Test & obj)
	{
		Test ret(0);
		cout << "执行+号重载函数" << endl;
		ret.i = i + obj.i;
		return ret;
	}
	bool operator && (const Test & obj)
	{
		cout << "执行&&重载函数" << endl;
		return i && obj.i;
	}
private:

};
//&&结合顺序是从左到右

void main()
{
	int a1 = 0;
	int a2 = 1;

	if (a1 && (a1 + a2))
	{
		cout << "有一个是假，则不在执行下一个表达式计算" << endl;
	}

	Test t1 = 0;
	Test t2 = 1;

	if (t1 && (t1 + t2))//t1&&t1.operator(t2)       t1.operator&&( t1.operator(t2)  )
	{
		cout << "两个函数都执行，而且先执行了+" << endl;

	}

	system("pause");
	return;
}
//所以  一般不将&&重载  ||也是

//如果if里面是 (t1 + t2) && t1  相当于 t1.operator+(t2) && t1 相当于 （t1.operator+(t2)).operator&&(t1)  其中t1.operator+(t2)是一个匿名对象