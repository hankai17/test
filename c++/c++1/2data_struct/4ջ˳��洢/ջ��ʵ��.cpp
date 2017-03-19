#include <iostream>
using namespace std;
#include "seqstack.h"
//栈是一种特殊的线性表
void main()
{
	int i = 0;
	int a[10];
	SeqStack * stack = NULL;
	stack = SeqStack_Create(10);
	if (stack == NULL)
	{
		return;
	}
	for (i = 0; i < 5; i++)
	{
		a[i] = i + 1;
		SeqStack_Push(stack,&a[i]);
	}

	printf("容量： %d\n", SeqStack_Capacity(stack));
	printf("长度：%d\n", SeqStack_Size(stack));
	printf("栈顶元素：%d\n", *(int *)(SeqStack_Size(stack))  );

	while (SeqStack_Size(stack)>0)
	{
		int tmp=*((int *)SeqStack_Pop(stack));
		printf("tmp:%d\n", tmp);
	}


	system("pause");
	return;
}