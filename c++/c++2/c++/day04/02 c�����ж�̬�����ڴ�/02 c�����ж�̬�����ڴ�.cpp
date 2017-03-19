#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;


class Person{
public:
	Person(){
		cout << "无参构造函数！" << endl;
		mName = "undefined!";
		mAge = 0;
	}

	Person(string name,int age){
		cout << "2个参构造函数！" << endl;
		mName = name;
		mAge = age;
	}

	~Person(){
	
	}
public:
	string mName;
	int mAge;
};

void test01(){
	Person* person = (Person*)malloc(sizeof(Person));
	if (person == NULL){
		//return 0;
	}
	//person->Init();

}

void test02(){
	

	//new在堆上分配内存，并且调用构造函数完成初始化
	Person* person1 = new Person;
	Person* person2 = new Person("John",20);

	//释放堆内存
	delete person1;
	delete person2;
	//delete在释放堆内存前会调用一次析构函数
}

int main(){

	test02();


	system("pause");
	return EXIT_SUCCESS;
}