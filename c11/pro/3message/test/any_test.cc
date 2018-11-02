#include "any.hh"

void test1() {
  Any n = std::string("hello");
  auto r = n.any_cast<std::string>();
  //auto r = n.any_cast<int>();
  std::cout<<"r="<<r<<std::endl;
}

void test2() {
  Any n;
  auto r = n.is_null();
  std::cout<<"r="<<r<<std::endl;
  std::string s1 = "hello";
  n = s1;
  n.any_cast<int>();
  Any n1 = 1;
  n1.is<int>();
}

int main() {
  //test1();
  test2();
  return 0;
}
