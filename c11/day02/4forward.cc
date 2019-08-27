#include <iostream>

void PrintT(int& t) {
  std::cout << "lvalue" << std::endl;
}

template <typename T>
void PrintT(T&& t) {
  std::cout << "rvalue" << std::endl;
}

template <typename T>
void TestForward(T&& v) { //可接受纯右值 左值
  PrintT(v);
  PrintT(std::forward<T>(v));
  PrintT(std::move(v));
}

void Test() {
  TestForward(1);  //l r r       //传入右值 函数接右值T为int

  int x = 1;
  TestForward(x); //l l r        //传入左值 函数接左值T为int&
  TestForward(std::forward<int>(x)); //l r r    //传入右值
}

int main(void) {
  Test();
  return 0;
}

//当TestForward 接左值时 T会被转化为int& 根据折叠规则 这个参数类型实际上会变成int&
//当TestForward 接右值时 T会被转化成int 因此这个参数类型实际上会变成int&&
//std::forward<T>(u)有两个参数 当T为左值引用时 u将被转换为T类型的左值 否则u将被转换为T类型右值
//https://blog.csdn.net/rankun1/article/details/78354153
