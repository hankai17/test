#include<iostream>
#include<stdexcept>
using namespace std;

class Person {
  public:
	Person() {
	  mAge = 0;
	}
	void setAge(int age) {
	  if (age < 0 || age > 100) {
		throw out_of_range("err, age between 0 and 100!");
	  }
	  mAge = age;
	}
  private:
	int mAge;
};

void test01() {
  Person person;
  try {
	person.setAge(1000);
  }
  catch (exception& ex) {
	cout << ex.what() << endl;
  }
}

int main(){
  test01();
  return 0;
}
