#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
class Person{
public:
	Person(char* name, int age){
		pName = (char*)malloc(sizeof(name));
		strcpy(pName, name);
		mAge = age;
	}
	~Person(){
		cout << "Îö¹¹º¯Êý!" << endl;
		if (pName != NULL){
			delete pName;
		}
	}
public:
	char* pName;
	int mAge;
};

void test(){
	void* person = new Person("john", 20);
	delete person;
}


int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}