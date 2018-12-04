#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Person{
public:
	Person(string name,int age){
		this->mName = name;
		this->mAge = age;
	}

	bool operator==(const Person& person){
		if (this->mName == person.mName && this->mAge == person.mAge){
			return true;
		}
		return false;
	}

	bool operator!=(const Person& person){
		if (this->mName != person.mName || this->mAge != person.mAge){
			return true;
		}
		return false;
	}

private:
	string mName;
	int mAge;
};

int main(){

	Person person1("John",20);
	Person person2("John",30);

	if (person1 == person2){
		cout << "两个人长得一模一样!" << endl;
	}

	if (person1 != person2){
		cout << "这俩人长得不一样!" << endl;
	}

	system("pause");
	return EXIT_SUCCESS;
}