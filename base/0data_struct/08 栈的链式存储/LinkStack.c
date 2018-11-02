#include"LinkStack.h"

//初始化
LinkStack Init_LinkStack(){
	LStack* stack = (LStack*)malloc(sizeof(LStack));
	stack->header.next = NULL;
	stack->size = 0;
	return stack;
}
//压栈
void Push_LinkStack(LinkStack stack, StackNode* data){
	if (stack == NULL){
		return;
	}
	if (data == NULL){
		return;
	}

	LStack* lstack = (LStack*)stack;
	data->next = lstack->header.next;
	lstack->header.next = data;
	lstack->size++;
}
//出栈
void Pop_LinkStack(LinkStack stack){
	if (stack == NULL){
		return;
	}
	LStack* lstack = (LStack*)stack;
	if (lstack->size == 0){
		return;
	}

	//第一个节点
	StackNode* pFirst = lstack->header.next;
	lstack->header.next = pFirst->next;
	lstack->size--;
}
//获得栈顶元素
void* Top_LinkStack(LinkStack stack){
	if (stack == NULL){
		return NULL;
	}
	LStack* lstack = (LStack*)stack;
	return lstack->header.next;
}
//大小
int Size_LinkStack(LinkStack stack){
	if (stack == NULL){
		return -1;
	}
	LStack* lstack = (LStack*)stack;
	return lstack->size;
}
//销毁栈
void Destroy_LinkStack(LinkStack stack){
	if (stack == NULL){
		return;
	}
	free(stack);
}