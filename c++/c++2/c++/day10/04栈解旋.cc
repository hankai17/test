#include<iostream>
#include<string>
using namespace std;

class Person {
  public:
	Person(string name) {
	  mName = name;
	  cout << mName << "obj created!" << endl;
	}
	~Person() {
	  cout << mName << "obj decreated!" << endl;
	}
  public:
	string mName;
};

void func() {
  Person p("aaa");
  throw 10;
}

int main() {
  try {
	func();
  }
  catch (int e) {
	cout << "abnormal catched!" << endl;
  }
  return 0;
}
