#include <iostream>

void func(auto a = 1);  //auto不能用于函数参数

struct Foo {
  auto var1_ = 0;  //auto不能用于非静态成员变量
  static const auto var2_ = 0;
};

template <typename T>
struct Bar {};

int main(void) {
  Bar<int> bar;
  Bar<auto> bb = bar;  //auto无法推导出模板参数

  int arr[10] = {0};
  auto rr[10] = arr;  //auto无法定义数组
  auto aa = arr;  //aa->int* 没问题
  return 0;
}

/* 
auto是类型指示符(type-specifier) 而非存储类型指示符(storage-class-specifiers) eg:static register mutable
cv限定符:(cv-qualifier) const_volatile

auto会把const 引用属性去掉  eg:auto f = e;(其中e是 int&类型的) 但推导出的f只是int类型的
auto x = 5 //5是const int类型而非int类型 但推倒出来是int类型const消失
当auto& 或者auto* const时 不会去掉引用属性
*/
