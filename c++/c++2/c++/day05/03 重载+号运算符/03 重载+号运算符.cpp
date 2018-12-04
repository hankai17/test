#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Person{
public:
	Person(int id,int age){
	    mID = id;
		mAge = age;
	}
	Person PersonPlusPerson(Person& person){
		int newid = this->mID + person.mID;
		int newage = this->mAge + person.mAge;
		Person newperson(newid, newage);
		return newperson;
	}

#if 0
	Person operator+(Person& p){
		cout << "1" << endl;
		int newid = this->mID + p.mID;
		int newage = this->mAge + p.mAge;
		Person newperson(newid, newage);
		return newperson;
	}
#endif

	//Ö»¶Á½Ó¿Ú
	int getID(){ return mID; }
	int getAge(){ return mAge; }

public:
	int mID;
	int mAge;
};

#if 1
Person operator+(Person& p1,Person& p2){
	cout << "2" << endl;
	int newid = p1.mID + p2.mID;
	int newage = p1.mAge + p2.mAge;
	Person newperson(newid, newage);
	return newperson;
}
#endif


void test01(){
	Person p1(10, 10);
	Person p2(20, 20);

	Person p3 =  p1.PersonPlusPerson(p2);
	Person p4 = p1 + p2;  // p1.operator+(p2)

	cout << "ID:" << p4.getID() << " Age:" << p4.getAge() << endl;
}


int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}