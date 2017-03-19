#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"LinkList.hpp"
#include<string>

class Person{
public:
	Person(){}
	Person(string name,int age){
		this->mName = name;
		this->mAge = age;
	}
	bool operator==(const Person& p){
		return p.mName == this->mName && p.mAge == this->mAge;
	}
public:
	string mName;
	int mAge;
};

void print(Person& p){
	cout << "Name:" << p.mName << " Age:" << p.mAge << endl;
}
struct print02{
	void operator()(Person& p){
		cout << "Name:" << p.mName << " Age:" << p.mAge << endl;
	}
};

int main(){


	//初始化链表
	LinkList<Person> list;
	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);
	Person p5("eee", 50);
	Person p6("fff", 60);
	//头部插入数据
	list.Push_Front(p1);
	list.Push_Front(p2);
	//尾插
	list.Push_Back(p3);
	list.Push_Back(p4);
	//指定位置插入
	list.Insert(2, p5);
	list.Insert(2, p6);
	//打印
	//list.Print(print);
	list.Print(print02());
	//头删
	list.Pop_Front();
	cout << "头删结果:" << endl;
	list.Print(print02());
	cout << "---------" << endl;
	//尾删
	list.Pop_Back();
	cout << "尾删结果:" << endl;
	list.Print(print02());
	cout << "---------" << endl;
	//指定位置删除
	list.RemoveByPos(2);
	cout << "指定删除位置2结果:" << endl;
	list.Print(print02());
	cout << "---------" << endl;
	//值删除
	list.RemoveByVal(p6);
	cout << "值删除结果:" << endl;
	list.Print(print02());
	cout << "---------" << endl;
	cout << "Size:" << list.Size() << endl;

	system("pause");
	return EXIT_SUCCESS;
}