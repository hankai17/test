#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"SeqStack.h"

int IsNumber(char ch){
	return ch >= '0' && ch <= '9';
}

int IsOperator(char ch){
	return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

int Result(int leftNum,int rightNum,char myOperator){

	int ret = 0;
	switch (myOperator){
	case '+':
		ret = leftNum + rightNum;
		break;
	case '-':
		ret = leftNum - rightNum;
		break;
	case '*':
		ret = leftNum * rightNum;
		break;
	case '/':
		ret = leftNum / rightNum;
		break;
	}
	return ret;
}

typedef struct _MYNUMBER{
	int number;
}MyNumber;

int main(){


	char* str = "831-5*+";
	char* p = str;

	//初始化栈
	SeqStack stack = Init_SeqStack();

	while (*p != '\0'){
	
		//数字直接入栈
		if (IsNumber(*p)){
			MyNumber* number = (MyNumber*)malloc(sizeof(MyNumber));
			number->number = *p - '0';
			Push_SeqStack(stack, number);
		}

		//运算符号
		if (IsOperator(*p)){
			
			//从栈中弹出右操作数
			MyNumber* rightNum = (MyNumber*)Top_SeqStack(stack);
			Pop_SeqStack(stack);	
			//从栈中弹出左操作数
			MyNumber* leftNum = (MyNumber*)Top_SeqStack(stack);
			Pop_SeqStack(stack);

			//计算
			int ret = Result(leftNum->number,rightNum->number,*p);
			//结果压栈
			MyNumber* MySum = (MyNumber*)malloc(sizeof(MyNumber));
			MySum->number = ret;
			Push_SeqStack(stack, MySum);

			free(leftNum);
			free(rightNum);
		}

		p++;
	}

	//取出栈中元素
	MyNumber* result = (MyNumber*)Top_SeqStack(stack);
	printf("结果:%d\n",result->number);
	Pop_SeqStack(stack);
	free(result);

	system("pause");
	return EXIT_SUCCESS;
}