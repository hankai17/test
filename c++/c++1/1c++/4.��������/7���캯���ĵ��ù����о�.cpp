#include <iostream>
using namespace std;

class Test
{
public:
	Test()
	{
		a = 0;
		b = 0;
		cout << "无参构造函数" << endl;
	}
	Test(int _a)
	{
		a = _a;
		b = 0;
	}
	void printT()
	{
		cout << "a:" << a << "b" << b << endl;
	}
	Test(const Test & obj)
	{
		a = obj.a + 100;
		b = obj.b + 100;

	}
	~Test()
	{
		cout << "析构函数 对象生命周期结束时 c++编译器自动调用" << endl;
	}
private:
	int a;
	int b;

};
//当类中定义了拷贝构造函数时 c++编译器不会提供无参构造函数
//当          有参               
// 结论：在定义类时 只要写了构造函数 则必须要用
void main()
{
	Test t1;//无参构造函数//当类中定义了拷贝构造函数时 c++编译器不会提供无参构造函数
	system("pause");
	return;
}