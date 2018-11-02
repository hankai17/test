#include"LinkStack.h"

//初始化
LinkStack Init_LinkStack(){

	LStack* stack = (LStack*)malloc(sizeof(LStack));
	if (stack == NULL){
		return NULL;
	}

	stack->head = NULL;
	stack->size = 0;

	return stack;
}
//压栈
void Push_LinkStack(LinkStack stack, void* data){

	if (stack == NULL){
		return;
	}

	if (data == NULL){
		return;
	}

	StackNode*  node = (StackNode*)data;
	LStack* sstack = (LStack*)stack;

	node->next = sstack->head;
	sstack->head = node;

	sstack->size++;
}
//出栈
void Pop_LinkStack(LinkStack stack){

	if (stack == NULL){
		return;
	}
	LStack* sstack = (LStack*)stack;
	if (sstack->size == 0){
		return;
	}

	//缓存下第一个节点
	StackNode* pFirst = sstack->head;
	sstack->head = pFirst->next;
	
	sstack->size--;
}
//获得栈顶元素
void* Top_LinkStack(LinkStack stack){
	if (stack == NULL){
		return NULL;
	}
	LStack* sstack = (LStack*)stack;
	if (sstack->size == 0){
		return NULL;
	}

	return sstack->head;
}
//大小
int Size_LinkStack(LinkStack stack){
	if (stack == NULL){
		return -1;
	}
	LStack* sstack = (LStack*)stack;
	return sstack->size;
}
//销毁
void Destroy_LinkStack(LinkStack stack){
	if (stack == NULL){
		return;
	}

	free(stack);
}