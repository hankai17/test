#include <iostream>

void func(auto a = 1);  //error:auto不能用于函数参数

struct Foo
{
	auto var1_ = 0;  //error:auto不能用于非静态成员变量
	static const auto var2_ = 0;
};

template <typename T>
struct Bar {};

int main(void)
{
	int arr[10] = {0};
	auto aa = arr;  //OK: aa -> int *
	auto rr[10] = arr;  //error:auto无法定义数组
	Bar<int> bar;
	Bar<auto> bb = bar;  //error:auto无法推导出模板参数

	system("pause");
	return 0;
}
/* auto x = 5; 5是const int类型,而不是int类型, 但推倒出来是int类型,const消失
 ** auto是类型指示符(type-specifier) 而非存储类型指示符(storage-class-specifiers)eg:static,register,mutable
 cv限定符:(cv-qualifier)const_volatile
 auto会把const,引用,属性去掉  eg: auto f = e;(其中e是 int&类型的) 但推导出的f只是int类型的
 当auto& 或者auto* const时,不会去掉引用属性
 */
