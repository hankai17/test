#include<iostream>
#include<exception>
using namespace std;

class MyOutOfRange : public exception {
  public:
	MyOutOfRange(const char* errorInfo) {
	  pError = new char[strlen(errorInfo) + 1];
	  strcpy(pError, errorInfo);
	}
	virtual  ~MyOutOfRange() _NOEXCEPT {
	  if (pError != NULL) {
		delete[] pError;
		pError = NULL;
	  }
	}
	virtual const char *  what() const {
	  return pError;
	}
  private:
	char* pError;
};

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

int main() {
  //test01();
  char buf[1024] = {0};
  cin.ignore(2); //ºöÂÔ»º³åÇøµ±Ç°×Ö·û
  cin.get(buf, 1024);
  cout << buf << endl;
  return 0;
}
