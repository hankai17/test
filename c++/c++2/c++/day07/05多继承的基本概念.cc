#include<iostream>
using namespace std;

class Base1 {
  public:
	void func1() {
	  cout << "Base1::func1" << endl;
	}
	void func2() {
	  cout << "Base1::func2" << endl;
	}
};

class Base2 {
  public:
	void func1() {
	  cout << "Base2::func1" << endl;
	}
	void func3() {
	  cout << "Base2::func3" << endl;
	}
	void func4() {
	  cout << "Base2::func4" << endl;
	}
};

//多继承可能会出现二义性
class Derived : public Base1,public Base2 {
  public:
	void MyFunc() {
	  Base1::func1();
	  Base2::func1();
	  func2();
	  func3();
	  func4();
	}
};

int main() {
  Derived d;
  d.MyFunc();
  return 0;
}
