#include <iostream>

class Foo{};
int& func_int_r(void);
int&& func_int_rr(void);
int func_int(void);

const int& func_cint_r(void); //左值
const int&& func_cint_rr(void); //x值
const int func_cint(void); //纯右值
const Foo func_cfoo(void); //纯右值

int main(void) {
  int x = 0;

  decltype(func_int_r()) a1 = x; //a1->int&  
  decltype(func_int_rr()) b1 = 0; //b1->int&&
  decltype(func_int()) c1 = 0; //c1->int

  decltype(func_cint_r()) a2 = x; //a2->const int&
  decltype(func_cint_rr()) b2 = 0; //b2->const int&&
  decltype(func_cint()) c2 = 0; //c2->int 纯右值 只有类类型可以携带cv限定符 其它一律忽略cv
  decltype(func_cfoo()) ff = Foo();  //ff->const Foo  //纯右值只有类类型可以携带cv限定符
  return 0;
}
