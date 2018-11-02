#include"CircleLinkList.h"

//初始化
CircleLinkList Init_CircleLinkList(){
	CLinkList* clist = (CLinkList*)malloc(sizeof(CLinkList));
	clist->size = 0;
	clist->header.next = &(clist->header);
	return clist;
}
//指定位置插入
void Insert_CircleLinkList(CircleLinkList clist, int pos, CircleLinkNode* data){
	if (clist == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	CLinkList* list = (CLinkList*)clist;
	if (pos < 0 || pos > list->size){
		pos = list->size;
	}
	
	//找到插入位置
	CircleLinkNode* pCurrent = &(list->header);
	int i = 0;
	for (; i < pos;i++){
		pCurrent = pCurrent->next;
	}
	data->next = pCurrent->next;
	pCurrent->next = data;
	list->size++;
}
//头部插入操作
void PushFront_CircleLinkList(CircleLinkList clist, CircleLinkNode* data){
	if (clist == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	Insert_CircleLinkList(clist, 0, data);
}
//尾部插入操作
void PushBack_CircleLinkList(CircleLinkList clist, CircleLinkNode* data){
	if (clist == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	CLinkList* list = (CLinkList*)clist;
	Insert_CircleLinkList(clist, list->size, data);
}
//指定位置删除
void RemoveByPos_CircleLinkList(CircleLinkList clist, int pos){
	if (clist == NULL){
		return;
	}
	CLinkList* list = (CLinkList*)clist;
	if (pos < 0 || pos > list->size - 1){
		return;
	}
	if (list->size == 0){
		return;
	}

	//查找被删除节点的前一个位置的节点
	CircleLinkNode* pCurrent = &(list->header);
	int i = 0;
	for (; i < pos;i++){
		pCurrent = pCurrent->next;
	}
	//缓存被删除节点
	CircleLinkNode* pDel = pCurrent->next;
	pCurrent->next = pDel->next;
	list->size--;
}
//头部删除操作
void PopFront_CircleLinkList(CircleLinkList clist){
	if (clist == NULL){
		return;
	}
	CLinkList* list = (CLinkList*)clist;
	if (list->size == 0){
		return;
	}
	RemoveByPos_CircleLinkList(clist, 0);
}
//尾部删除操作
void PopBack_CircleLinkList(CircleLinkList clist){
	if (clist == NULL){
		return;
	}
	CLinkList* list = (CLinkList*)clist;
	if (list->size == 0){
		return;
	}
	RemoveByPos_CircleLinkList(clist, list->size);
}
//值删除
void RemoveByVal_CircleLinkList(CircleLinkList clist, CircleLinkNode* data, DATA_COMPARE compare){
	if (clist == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	if (compare == NULL){
		return;
	}
	CLinkList* list = (CLinkList*)clist;
	if (list->size == 0){
		return;
	}

	//赋值指针变量
	CircleLinkNode* pPrev = &(list->header);
	CircleLinkNode* pCurrent = pPrev->next;
	while (pCurrent != NULL){
		if (compare(pCurrent,data)){
			pPrev->next = pCurrent->next;
			list->size--;
			break;
		}
		pPrev = pCurrent;
		pCurrent = pPrev->next;
	}

}

//获得指定位置元素
void* Get_CircleLinkList(CircleLinkList clist, int pos){
	if (clist == NULL){
		return NULL;
	}

	CLinkList* list = (CLinkList*)clist;
	if (pos < 0 || pos > list->size - 1){
		return NULL;
	}
	
	//赋值指针变量
	CircleLinkNode* pCurrent = &(list->header);
	int i = 0;
	for (; i < pos;i++){
		pCurrent = pCurrent->next;
	}

	return pCurrent->next;
}

//链表大小
int Size_CircleLinkList(CircleLinkList clist){
	if (clist == NULL){
		return -1;
	}
	CLinkList* list = (CLinkList*)clist;
	return list->size;
}

//打印链表
void Print_CircleLinkList(CircleLinkList clist, DATA_PRINT print){
	if (clist == NULL){
		return;
	}
	CLinkList* list = (CLinkList*)clist;
	//辅助指针变量
	CircleLinkNode* pCurrent = list->header.next;
	int i = 0;
	for (; i < list->size;i++){
		print(pCurrent);
		pCurrent = pCurrent->next;
		if (pCurrent == &(list->header)){
			pCurrent = pCurrent->next;
		}
	}

}
//销毁链表
void Destroy_CircleLinkList(CircleLinkList clist){
	if (clist == NULL){
		return;
	}
	free(clist);
}