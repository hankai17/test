#include"LinkQueue.h"


//初始化
LinkQueue Init_LinkQueue(){

	LQueue* queue = (LQueue*)malloc(sizeof(LQueue));
	if (queue == NULL){
		return NULL;
	}

	queue->header.next = NULL;
	queue->size = 0;

	return queue;
}
//入队  头部位置 当做 队尾
void Push_LinkQueue(LinkQueue queue, void* data){

	if (queue == NULL){
		return;
	}

	if (data == NULL){
		return;
	}

	QueueNode* node = (QueueNode*)data;
	LQueue* lqueue = (LQueue*)queue;

	//节点入链表
	node->next = lqueue->header.next;
	lqueue->header.next = node;

	lqueue->size++;
}
//出队  尾部位置当队头
void Pop_LinkQueue(LinkQueue queue){

	if (queue == NULL){
		return;
	}

	LQueue* lqueue = (LQueue*)queue;
	
	QueueNode* pCurrent = &(lqueue->header);
	for (int i = 0; i < lqueue->size - 1; i ++){
		pCurrent = pCurrent->next;
	}

	pCurrent->next = NULL;
	lqueue->size--;
}
//获得队头元素
void* Front_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return NULL;
	}
	LQueue* lqueue = (LQueue*)queue;

	QueueNode* pCurrent = &(lqueue->header);
	for (int i = 0; i < lqueue->size; i++){
		pCurrent = pCurrent->next;
	}

	return pCurrent;
}
//获得队尾元素
void* Back_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return NULL;
	}
	LQueue* lqueue = (LQueue*)queue;
	return lqueue->header.next;
}
//大小
int Size_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return -1;
	}
	LQueue* lqueue = (LQueue*)queue;
	return lqueue->size;
}
//销毁‘
void Destroy_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return;
	}
	free(queue);
}