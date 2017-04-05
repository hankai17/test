#include <iostream>

class Foo
{
	public:
		static const int Number = 0;
		int x;
};

int main(void)
{
	int n = 0;
	volatile const int &x = n;

	decltype(n) a = n;         //a -> int
	decltype(x) b = n;         //b -> const volatile int &

	decltype(Foo::Number) c = 0;  //c -> const int

	system("pause");
	return 0;
}
//auto是必须得定义变量 必须得初始化 必须在编译之前确定其类型
// 而decltype是在编译期间自动推倒 不是用于定义类型 而是仅仅获取得到一个类型
// decltype 推倒三规则 decltype(exp)
// 	1 exp是标示符 类访问表达式， 则decltype(exp) 和exp类型一致
//	2 exp是函数调用， 则decltype(exp) 和函数返回值类型一致
//	3 exp若是左值，则decltype(exp)是exp的左值引用 ，否则和exp类型一致
