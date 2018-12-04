#include<iostream>
using namespace std;

class A {
  public:
	A() { cout << "a" << endl; }
	A(int x) { cout << "a" << endl; }
	void func() { cout << "hello world!" << endl; }
	void func(int a) { cout << "A:" << a << endl; }
};

#if 0
class B : public A {
  public:
};

void test01() {
  B b;
  b.func();
  b.func(10);
}
#else

class B : public A {
  public:
	B() : A(10) { cout << "a" << endl; }
	int func(int a) {
	  cout << "A:" << a << endl;
	  return 0;
	}
};

void test01() {
  B b;
  b.func(10);

  //b.A::func();
  //b.A::func(10);

  //b.func();
  //b.func(10);
}

#endif


int main() {
  test01();
  return 0;
}
