#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
#include<queue>
using namespace std;

//队列容器 先进先出
void test01(){

	//创建队列
	queue<int> q;

	//向队列中添加元素  入队操作
	q.push(10);
	q.push(20);
	q.push(30);
	q.push(40);

	//队列不提供迭代器，更不支持随机访问	
	while (!q.empty()){
		//输出对头元素
		cout << q.front() << " ";
		//弹出对头元素
		q.pop();
	}
	cout << endl;

	cout << "size:" << q.size() << endl;



}

int main(){

	test01();

	system("pause");
	return EXIT_SUCCESS;
}