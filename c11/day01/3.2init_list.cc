#include <iostream>

class Foo {
  public:
	Foo(int){}

  private:
	Foo(const Foo&);
};

int main(void) {
  Foo a1(123);
  Foo a2 = a1;  //error C2248: “Foo::Foo”: 无法访问 private 成员(在“Foo”类中声明)
  Foo a3 = { 123 }; //just as init a1
  Foo a4{ 123 }; //just as init a1
  int a5 = { 3 };
  int a6 { 3 };

  return 0;
}

