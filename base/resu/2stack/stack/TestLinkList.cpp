#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;
#include"LinkStack.h"

struct Person{
	StackNode node;
	int ID;
	int Age;
};

int main(){

	//初始化链式栈
	LinkStack stack = Init_LinkStack();
	//创建数据
	struct Person p1 = { NULL, 10, 10 };
	struct Person p2 = { NULL, 20, 20 };
	struct Person p3 = { NULL, 30, 30 };
	struct Person p4 = { NULL, 40, 40 };
	struct Person p5 = { NULL, 50, 50 };
	struct Person p6 = { NULL, 60, 60 };
	//数据入栈
	Push_LinkStack(stack, &p1);
	Push_LinkStack(stack, &p2);
	Push_LinkStack(stack, &p3);
	Push_LinkStack(stack, &p4);
	Push_LinkStack(stack, &p5);
	Push_LinkStack(stack, &p6);
	//输出栈中元素
	while (Size_LinkStack(stack) > 0){
		//获得栈顶元素
		struct Person* person = (struct Person*)Top_LinkStack(stack);
		cout << "ID:" << person->ID << " Age:" << person->Age << endl;
		//弹出栈顶元素
		Pop_LinkStack(stack);
	}

	//销毁栈
	Destroy_LinkStack(stack);

	system("pause");
	return EXIT_SUCCESS;
}