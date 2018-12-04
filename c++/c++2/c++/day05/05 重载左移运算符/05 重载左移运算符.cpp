#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Person{
	friend ostream& operator<<(ostream& out, Person& person);
public:
	Person(int id,int age){
		mID = id;
		mAge = age;
	}
	void ShowPerson(){
		cout << "ID:" << mID << " Age:" << mAge << endl;
	}
private:
	int mID;
	int mAge;
};

ostream& operator<<(ostream& out, Person& person){
	out << "ID:" << person.mID << " Age:" << person.mAge;
	return out;
}

int main(){

	Person person(10,20);

	person.ShowPerson();

	cout << "person:" << person << "-------------------" << endl;

#if 0
	cout << 10; //cout.operator<<(int)
	cout << "aaaa" << endl; //cout.operator<<(const char*)

#endif


	system("pause");
	return EXIT_SUCCESS;
}