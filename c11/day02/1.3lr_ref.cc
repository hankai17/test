#include<iostream>

void print_value(int& i) {
  std::cout<<"lvalue="<<i<<std::endl;
}

void print_value(int&& i) {
  std::cout<<"rvalue="<<i<<std::endl;
}

void forward(int&& i) {
  print_value(i);
}

int main()
{
  int i = 0;
  print_value(i); //左值
  print_value(1); //右值
  forward(2); //右值变成命名对象 是个左值
  return 0;
}
