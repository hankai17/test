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
//入队
void Push_LinkQueue(LinkQueue queue, QueueNode* data){
	if (queue == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	LQueue* mqueue = (LQueue*)queue;
	//找到最后一个元素
	QueueNode* pCurrent = &(mqueue->header);
	int i = 0;
	for (; i < mqueue->size; i++){
		pCurrent = pCurrent->next;
	}
	//新元素入队列
	data->next = pCurrent->next;
	pCurrent->next = data;
	mqueue->size++;
}
//出队
void Pop_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return;
	}
	LQueue* mqueue = (LQueue*)queue;
	if (mqueue->size == 0){
		return;
	}
	QueueNode* pFirst = mqueue->header.next;
	mqueue->header.next = pFirst->next;
	mqueue->size--;
}
//获得队头元素
void* Front_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return NULL;
	}
	LQueue* mqueue = (LQueue*)queue;
	return mqueue->header.next;
}
//获得队尾元素
void* Back_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return NULL;
	}
	LQueue* mqueue = (LQueue*)queue;
	//找到最后一个元素
	QueueNode* pCurrent = &(mqueue->header);
	int i = 0;
	for (; i < mqueue->size; i++){
		pCurrent = pCurrent->next;
	}
	return pCurrent;
}
//队列大小
int Size_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return -1;
	}
	LQueue* mqueue = (LQueue*)queue;
	return mqueue->size;
}
//销毁队列
void Destroy_LinkQueue(LinkQueue queue){
	if (queue == NULL){
		return;
	}
	free(queue);
}
