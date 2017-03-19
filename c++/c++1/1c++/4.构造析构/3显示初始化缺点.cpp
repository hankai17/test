#include <iostream>
using namespace std;

class Test3
{
public:
	void init(int _a, int _b)
	{
		a = _a;
		b = _b;
	}
private:
	int a;
	int b;
};

void main()
{
	Test3 t1;//编译器自动提供构造 析构 拷贝构造函数 里面是空的
	
	int a = 10;
	int b = 11;
	t1.init(a, b);//这样显示的初始化很麻烦

	system("pause");
	return;
}