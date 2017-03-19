#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stack>
using namespace std;

//1. stack容器
void test01(){

	//创建栈容器 栈容器必须符合先进后出
	stack<int> s;
	stack<int> s2;
	s2 = s;

	//向栈中添加元素，叫做 压栈 入栈
	s.push(10);
	s.push(20);
	s.push(30);

	while (s.size() > 0){
		//输出栈顶元素
		cout << s.top() << " ";
		//弹出栈顶元素
		s.pop();
	}
	cout << "size:" << s.size() << endl;
}

int main(){

	test01();


	system("pause");
	return EXIT_SUCCESS;
}