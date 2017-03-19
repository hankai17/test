#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

template<class T>
class Person{
	//重载左移操作符
	template<class T1> friend ostream& operator<<(ostream& out, Person<T1>& person);
	//普通友元函数
	template<class T2> friend void ShowPerson(Person<T2>& person);
public:
	Person(T property);
	void PirntPerson();

private:
	T mProperty;
};


template<class T>
Person<T>::Person(T property){
	this->mProperty = property;
}

template<class T>
void Person<T>::PirntPerson(){
	cout << "Property:" << mProperty << endl;
}

//友元函数
template<class T>
ostream& operator<<(ostream& out, Person<T>& person){
	out << person.mProperty << endl;
	return out;
}

template<class T>
void ShowPerson(Person<T>& person){
	cout << "Property:" << person.mProperty << endl;
}
void test(){
	Person<int> person(10);
	person.PirntPerson();
	cout << person;
	ShowPerson(person);
}


int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}