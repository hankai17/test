#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdexcept>
using namespace std;

class Person{
public:
	Person(){
		mAge = 0;
	}
	void setAge(int age){
		if (age < 0 || age > 100){
			throw out_of_range("����Ӧ����0-100,�����벻��ȷ!");
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

	test01();

	system("pause");
	return EXIT_SUCCESS;
}