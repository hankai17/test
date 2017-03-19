#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"SeqQueue.hpp"
#include<string>

class Person{
public:
	Person(){}
	Person(string name, int age){
		this->mName = name;
		this->mAge = age;
	}
public:
	string mName;
	int mAge;
};

void test(){

	//初始化队列
	SeqQueue<Person> queue;
	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);
	Person p5("eee", 50);
	Person p6("fff", 60);
	//压栈
	queue.Push(p1);
	queue.Push(p2);
	queue.Push(p3);
	queue.Push(p4);
	queue.Push(p5);
	queue.Push(p6);
	//输出队头队尾元素
	cout << "Name:" << queue.Front().mName << " Age:" << queue.Front().mAge << endl;
	cout << "Name:" << queue.Back().mName << " Age:" << queue.Back().mAge << endl;
	cout << "-----------------" << endl;
	//输出
	while (queue.Size() > 0){
		//获得栈顶元素
		cout << "Name:" << queue.Front().mName << " Age:" << queue.Front().mAge << endl;
		queue.Pop();
	}

}

int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}