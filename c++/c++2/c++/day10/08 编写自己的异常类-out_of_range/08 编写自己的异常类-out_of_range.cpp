#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<exception>
using namespace std;

class MyOutOfRange : public exception{
public:
	MyOutOfRange(const char* errorInfo){
		pError = new char[strlen(errorInfo) + 1];
		strcpy(pError, errorInfo);
	}
	virtual  ~MyOutOfRange() _NOEXCEPT{
		if (pError != NULL){
			delete[] pError;
			pError = NULL;
		}
	}
	virtual const char *  what() const{
		return pError;
	}
private:
	char* pError;
};

class Person{
public:
	Person(){
		mAge = 0;
	}
	void setAge(int age){
		if (age < 0 || age > 100){
			throw MyOutOfRange("����Ӧ����0-100,�����벻��ȷ!");
		}
		mAge = age;
	}
private:
	int mAge;
};

void test01(){
	Person person;
	try{
		person.setAge(1000);
	}
	catch (exception& ex){
		cout << ex.what() << endl;
	}
}

int main(){

	//test01();

	char buf[1024] = { 0 };
	cin.ignore(2); //���Ի�������ǰ�ַ�
	cin.get(buf, 1024);
	cout << buf << endl;


	system("pause");
	return EXIT_SUCCESS;
}