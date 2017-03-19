#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

template<class T>
class Person{
	//重载左移操作符
	friend ostream& operator<<(ostream& out,Person& person){
		out << "Property:" << person.mProperty << endl;
		return out;
	}
	//普通友元函数
	friend void ShowPerson(Person& person){
		cout << "Property:" << person.mProperty << endl;
	}

public:
	Person(T property){
		mProperty = property;
	}
	void PirntPerson(){
		cout << "Property:" << mProperty << endl;
	}

private:
	T mProperty;
};


void test(){
	
	Person<int> person(10);

	cout << person << endl;
	person.PirntPerson();
	ShowPerson(person);

}


int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}