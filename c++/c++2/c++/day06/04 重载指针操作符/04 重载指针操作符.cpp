#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Person{
public:
	Person(){
		mID = 10;
	}
	void ShowPerson(){
		cout << "ID:" << mID << endl;
	}
private:
	int mID;
};

class SmartPointer{
public:
	SmartPointer(Person* person){
		pPerson = person;
	}
	Person* operator->(){
		return pPerson;
	}

	Person& operator*(){
		return *pPerson;
	}
	~SmartPointer(){
		cout << "Îö¹¹º¯Êý!" << endl;
		if (pPerson != NULL){
			delete pPerson;
			pPerson = NULL;
		}
	}
private:
	Person* pPerson;
};


void test01(){
	
	SmartPointer person(new Person);
	person->ShowPerson();
	(*person).ShowPerson();

}


int main(){

	test01();


	system("pause");
	return EXIT_SUCCESS;
}