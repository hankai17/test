#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Person{
public:
	Person(char* name,int age){
		pName = (char*)malloc(strlen(name) + 1);
		strcpy(pName, name);
		mAge = age;
	}

	//增加拷贝构造函数
	Person(const Person& person){
		pName = (char*)malloc(strlen(person.pName) + 1);
		strcpy(pName,person.pName);
		mAge = person.mAge;
	}
	~Person(){
		if (pName != NULL){
			free(pName);
			pName = NULL;
		}
	}
public:
	char* pName;
	int mAge;
};

void test01(){

	Person person1("Obama",20);
	Person person2(person1);
}

int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}