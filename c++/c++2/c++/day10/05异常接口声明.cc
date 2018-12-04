#include<iostream>
using namespace std;

//只能抛出const char*类型和char类型异常 只是一个说明 但实际情况下可以跑出任何异常
void func1() throw(const char*, char) {
  throw 10;
}

void test01() {
  try {
	func1();
  }
  catch (int) {
	cout << "catch int abnormal!" << endl;
  }
  catch (...) {
	cout << "other abnormal!" << endl;
  }
}

//什么类型异常都不能抛出
void func2() throw() {
  throw 10;
}

void test02() {
  try {
	func2();
  }
  catch (int) {
	cout << "catch int abnormal!" << endl;
  }
}

//可以抛出任何类型的异常
void func3() {
  throw 10;
}

void test03() {
  try {
	func3();
  }
  catch (int) {
	cout << "catch int abnormal!" << endl;
  }
}

int main() {
  test01();
  //test02();
  //test03();
  return 0;
}
