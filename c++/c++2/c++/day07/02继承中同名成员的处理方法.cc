#include<iostream>
using namespace std;

class A {
  public:
	A() { mA = 10; }
	int mA;
};

class B : public A {
  public:
	B() { mA = 20; }
	int mA;
};

void test01() {
  B b;
  cout << b.A::mA << endl;
}

int main() {
  test01();
  return 0;
}
