#include"SeqStack.h"

//初始化
SeqStack Init_SeqStack(){
	SStack* stack = (SStack*)malloc(sizeof(SStack));
	if (stack == NULL){
		return NULL;
	}
	stack->size = 0;
	for (int i = 0; i < MAXSIZE;i++){
		stack->data[i] = NULL;
	}
	return stack;
}
//压栈
void Push_SeqStack(SeqStack stack, void* data){
	if (stack == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	SStack* sstack = (SStack*)stack;
	if (sstack->size == MAXSIZE){
		return;
	}
	sstack->data[sstack->size] = data;
	sstack->size++;
}
//出栈
void Pop_SeqStack(SeqStack stack){
	if (stack == NULL){
		return;
	}
	SStack* sstack = (SStack*)stack;
	if (sstack->size == 0){
		return;
	}
	sstack->size--;
}
//获得栈顶元素
void* Top_SeqStack(SeqStack stack){
	if (stack == NULL){
		return NULL;
	}
	SStack* sstack = (SStack*)stack;
	if (sstack->size == 0){
		return NULL;
	}
	return sstack->data[sstack->size - 1];
}
//大小
int Size_SeqStack(SeqStack stack){
	if (stack == NULL){
		return -1;
	}
	SStack* sstack = (SStack*)stack;
	return sstack->size;
}
//销毁栈
void Destroy_SeqStack(SeqStack stack){
	if (stack == NULL){
		return;
	}
	free(stack);
}