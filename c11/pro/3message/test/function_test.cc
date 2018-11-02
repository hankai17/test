#include "function_traits.hh"
#include<string>
#include<iostream>

using namespace std;

template<typename T>
void print_type() {
  cout<<typeid(T).name()<<endl;
}

float (*cast_func) (string, int);
float free_function(const string& a, int b) {
  return (float)(a.size()/b);
}

struct AA {
  int f(int a, int b) volatile {
	return a + b;
  }
  int operator() (int) const {
	return 0;
  }
};

void test1() {
  cout<<function_traits<function<int(int, double, char, bool)> >::arity<<endl;
  cout<< typeid(function_traits<function<int(int)> >::function_type).name() <<endl;
  cout<< typeid(function_traits<function<int(int)> >::return_type).name() <<endl;
  cout<< typeid(function_traits<function<int(int)> >::stl_function_type).name() <<endl;
  cout<< typeid(function_traits<function<int(int)> >::pointer).name() <<endl;

}
/*
void test2() {
  function<int(int)> f = [](int a) {return a;};
  print_type<function_traits<function<int(int)> >::function_type>(); //int __cdcl(int)
  //print_type<function_traits<function<int(int)> >::arg_type<0>>(); //int
  print_type<function_traits<decltype(f) >::function_type>(); //int __cdcl(int)
  print_type<function_traits<decltype(free_function) >::function_type>(); //float __cdcl...

  print_type<function_traits<decltype(cast_func) >::function_type>(); //float __cdcl...
  print_type<function_traits<AA >::function_type>(); //int cdcl(int)

  using T = decltype(&AA::f);
  print_type<T>(); //int (_thiscall AA::*)(int int) volatile
  print_type<function_traits<decltype(&AA::f)>::function_type>(); //int __cdcl(int, int)
  //static_assert(is_same<function_traits<decltype(f)>::return_type, int>::value, "");
}
*/

int main() {
  test1();
  return 0;
}
