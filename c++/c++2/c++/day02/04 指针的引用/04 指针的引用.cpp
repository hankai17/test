#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//指针引用基本语法
void test01(){
	int* p = NULL;
	int*& pRef = p;
}

struct Person{
	int mAge;
};


//给指针分配空间并且初始化
void AllocateAndInitByPointer(Person** person){

	//分配空间
	*person = (Person*)malloc(sizeof(Person));
	//初始化
	(*person)->mAge = 100;
}

//释放内存
void FreeSpaceByPointer(Person** person){
	if (*person != NULL){
		free(*person);
		*person = NULL;
	}
}

//指针引用
void AllocateAndInitByReference(Person*& person){
	person = (Person*)malloc(sizeof(Person));
	person->mAge = 200;
}

//释放内存
void FreeSpaceByReference(Person*& person){
	if (person != NULL){
		free(person);
		person = NULL;
	}
}

int main(){

	Person* person = NULL;
	AllocateAndInitByPointer(&person);
	cout << "Age:" << person->mAge << endl;
	FreeSpaceByPointer(&person);
	//2. 指针的应用
	AllocateAndInitByReference(person);
	cout << "Age:" << person->mAge << endl;
	FreeSpaceByReference(person);

	system("pause");
	return EXIT_SUCCESS;
}