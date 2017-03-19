#include"DoubleLinkList.h"

//初始化
DoubleLinkList Init_DoubleLinkList(){
	DLinkList* dlist = (DLinkList*)malloc(sizeof(DLinkList));
	dlist->header.next = NULL;
	dlist->header.prev = NULL;
	dlist->size = 0;
	return dlist;
}
//指定位置插入
void Insert_DoubleLinkList(DoubleLinkList dlist, int pos, DoubleLinkNode* data){
	if (dlist == NULL){
		return;
	}
	DLinkList* list = (DLinkList*)dlist;
	if (pos < 0 || pos > list->size){
		pos = list->size;
	}

	//判断链表是否为空
	if (list->size == 0){
		data->next = list->header.next;
		data->prev = &(list->header);
		list->header.next = data;
		list->size++;
		return;
	}

	//查找插入位置的前一个位置
	DoubleLinkNode* pCurrent = &(list->header);
	int i = 0;
	for (; i < pos;i++){
		pCurrent = pCurrent->next;
	}

	//判断是否在尾部插入节点
	if (pos == list->size){
		data->next = pCurrent->next;
		data->prev = pCurrent;
		pCurrent->next = data;
		list->size++;
		return;
	}

	//其他插入情况
	data->next = pCurrent->next;
	data->prev = pCurrent;
	pCurrent->next->prev = data;
	pCurrent->next = data;
	list->size++;
}
//头部插入操作
void PushFront_DoubleLinkList(DoubleLinkList dlist, DoubleLinkNode* data){
	if (dlist == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	Insert_DoubleLinkList(dlist, 0, data);
}
//尾部插入操作
void PushBack_DoubleLinkList(DoubleLinkList dlist, DoubleLinkNode* data){
	if (dlist == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	DLinkList* list = (DLinkList*)dlist;
	Insert_DoubleLinkList(dlist, list->size, data);
}
//指定位置删除
void RemoveByPos_DoubleLinkList(DoubleLinkList dlist, int pos){
	if (dlist == NULL){
		return;
	}
	DLinkList* list = (DLinkList*)dlist;
	if (pos < 0 || pos > list->size - 1){
		return;
	}

	if (list->size == 0){
		return;
	}

	//判断链表是否为1个节点
	if (list->size == 1){
		list->header.next = NULL;
		list->size--;
		return;
	}

	//查找删除节点的前一个节点
	DoubleLinkNode* pCurrent = &(list->header);
	int i = 0;
	for (; i < pos;i++){
		pCurrent = pCurrent->next;
	}

	//判断删除的节点是否是最后一个节点
	if (pos == list->size - 1){
		pCurrent->next = NULL;
		list->size--;
		return;
	}

	//其他删除情况
	//缓存待删除节点
	DoubleLinkNode* pDel = pCurrent->next;
	pDel->next->prev = pCurrent;
	pCurrent->next = pDel->next;
	list->size--;
}
//头部删除操作
void PopFront_DoubleLinkList(DoubleLinkList dlist){
	if (dlist == 0){
		return;
	}
	DLinkList* list = (DLinkList*)dlist;
	if (list->size == 0){
		return;
	}
	RemoveByPos_DoubleLinkList(dlist,0);
}
//尾部删除操作
void PopBack_DoubleLinkList(DoubleLinkList dlist){
	if (dlist == 0){
		return;
	}
	DLinkList* list = (DLinkList*)dlist;
	if (list->size == 0){
		return;
	}
	RemoveByPos_DoubleLinkList(dlist, list->size - 1);
}
//链表大小
int Size_DoubleLinkList(DoubleLinkList dlist){
	if (dlist == NULL){
		return -1;
	}
	DLinkList* list = (DLinkList*)dlist;
	return list->size;
}
//正向打印链表
void Print_DoubleLinkList(DoubleLinkList dlist, DATA_PRINT print){
	if (dlist == NULL){
		return;
	}
	DLinkList* list = (DLinkList*)dlist;
	//辅助指针
	DoubleLinkNode* pCurrent = list->header.next;
	while (pCurrent != NULL){
		print(pCurrent);
		pCurrent = pCurrent->next;
	}
}
//反向打印链表
void RPrint_DoubleLinkList(DoubleLinkList dlist, DATA_PRINT print){
	if (dlist == NULL){
		return;
	}
	DLinkList* list = (DLinkList*)dlist;
	//找到尾节点
	DoubleLinkNode* pCurrent = &(list->header);
	int i = 0;
	for (; i < list->size;i++){
		pCurrent = pCurrent->next;
	}

	//逆序打印
	while (pCurrent != &(list->header)){
		print(pCurrent);
		pCurrent = pCurrent->prev;
	}
}
//销毁链表
void Destroy_DoubleLinkList(DoubleLinkList dlist){
	if (dlist == NULL){
		return;
	}
	free(dlist);
}