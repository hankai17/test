#include"SeqQueue.h"


//初始化
SeqQueue Init_SeqQueue(){
	SQueue* queue = (SQueue*)malloc(sizeof(SQueue));
	if (queue == NULL){
		return NULL;
	}
	queue->size = 0;
	return queue;
}
//入队
void Push_SeqQueue(SeqQueue queue, void* data){
	if (queue == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	SQueue* mqueue = (SQueue*)queue;
	if (mqueue->size == MAXSIZE){
		return;
	}
	
	//移动元素
	int i = mqueue->size - 1;
	for (; i >= 0; i --){
		mqueue->data[i + 1] = mqueue->data[i];
	}
	mqueue->data[0] = data;
	mqueue->size++;
}
//出队
void Pop_SeqQueue(SeqQueue queue){
	if (queue == NULL){
		return;
	}
	SQueue* mqueue = (SQueue*)queue;
	if (mqueue->size == 0){
		return;
	}
	mqueue->size--;
}
//获得队头元素
void* Front_SeqQueue(SeqQueue queue){
	if (queue == NULL){
		return NULL;
	}
	SQueue* mqueue = (SQueue*)queue;
	if (mqueue->size == 0){
		return NULL;
	}
	return mqueue->data[mqueue->size-1];
}
//获得队尾元素
void* Back_SeqQueue(SeqQueue queue){
	if (queue == NULL){
		return NULL;
	}
	SQueue* mqueue = (SQueue*)queue;
	if (mqueue->size == 0){
		return NULL;
	}
	return mqueue->data[0];
}
//获得队列的长度
int Size_SeqQueue(SeqQueue queue){
	if (queue == NULL){
		return -1;
	}
	SQueue* mqueue = (SQueue*)queue;
	return mqueue->size;
}
//销毁队列
void Destroy_SeqQueue(SeqQueue queue){
	if (queue == NULL){
		return;
	}
	free(queue);
}