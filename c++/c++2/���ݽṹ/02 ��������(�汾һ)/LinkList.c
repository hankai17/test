#include"LinkList.h"

//初始化
LinkList Init_LinkList(){
	LList* list = (LList*)malloc(sizeof(LList));
	list->size = 0;
	list->header.next = NULL;
	return list;
}
//指定位置插入
void Insert_LinkList(LinkList list, int pos, void* data){
	if (list == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	//判断位置是否越界 位置从0开始,如果越界，默认插入到尾部
	if (pos < 0 || pos > mlist->size){
		pos = mlist->size;
	}

	//查找插入位置
	LinkNode* pCurrent = &(mlist->header);
	int i = 0;
	for (; i < pos;i++){
		pCurrent = pCurrent->next;
	}

	//创建新节点
	LinkNode* newnode = (LinkNode*)malloc(sizeof(LinkNode));
	newnode->data = data;
	newnode->next = NULL;

	//新节点入链表
	newnode->next = pCurrent->next;
	pCurrent->next = newnode;
	mlist->size++;
}
//头部插入操作
void PushFront_LinkList(LinkList list, void* data){
	if (list == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	Insert_LinkList(list,0,data);
}
//尾部插入操作
void PushBack_LinkList(LinkList list, void* data){
	if (list == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	Insert_LinkList(list, mlist->size, data);
}
//指定位置删除
void RemoveByPos_LinkList(LinkList list, int pos){
	if (list == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	if (pos < 0 || pos >= mlist->size){
		return;
	}
	//判断链表是否为空
	if (mlist->size == 0){
		return;
	}

	//查找删除节点的前一个节点
	LinkNode* pCurrent = &(mlist->header);
	int i = 0;
	for (; i < pos;i++){
		pCurrent = pCurrent->next;
	}
	//缓存待删除节点
	LinkNode* pDel = pCurrent->next;
	//重新连接被删除节点前驱和后继节点
	pCurrent->next = pDel->next;
	//释放被删除节点内存
	free(pDel);
	mlist->size--;
}
//头部删除操作
void PopFront_LinkList(LinkList list){
	if (list == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	if (mlist->size == 0){
		return;
	}
	RemoveByPos_LinkList(list, 0);
}
//尾部删除操作
void PopBack_LinkList(LinkList list){
	if (list == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	if (mlist->size == 0){
		return;
	}
	RemoveByPos_LinkList(list, mlist->size - 1);
}
//值删除
void RemoveByVal_LinkList(LinkList list, void* data, DATA_COMPARE compare){

	if (list == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	if (compare == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	if (mlist->size == 0){
		return;
	}

	//查找删除节点
	LinkNode* pPrev = &(mlist->header);
	LinkNode* pCurrent = pPrev->next;
	while (pCurrent != NULL){
		if (compare(pCurrent->data,data)){
			pPrev->next = pCurrent->next;
			//释放删除节点内存
			free(pCurrent);
			mlist->size--;
			break;
		}
		pPrev = pCurrent;
		pCurrent = pCurrent->next;
	}
	
}
//获得指定位置元素
void* Get_LinkList(LinkList list, int pos){

	if (list == NULL){
		return NULL;
	}
	LList* mlist = (LList*)list;
	if (pos < 0 || pos > mlist->size){
		return NULL;
	}

	//查找pos前一个位置的节点
	LinkNode* pCurrent = &(mlist->header);
	for (int i = 0; i < pos;i++){
		pCurrent = pCurrent->next;
	}

	return pCurrent->next;
}
//链表大小
int Size_LinkList(LinkList list){
	if (list == NULL){
		return -1;
	}
	LList* mlist = (LList*)list;
	return mlist->size;
}
//遍历链表
void Print_LinkList(LinkList list, DATA_PRINT print){	
	if (list == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	//赋值指针
	LinkNode* pCurrent = mlist->header.next;
	while (pCurrent!=NULL){
		print(pCurrent->data);
		pCurrent = pCurrent->next;
	}
}
//销毁链表
void Destroy_LinkList(LinkList list){
	if (list == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	LinkNode* pCurrent = mlist->header.next;
	while (pCurrent != NULL){
		//缓存待删除节点下一个节点
		LinkNode* pNext = pCurrent->next;
		free(pCurrent);
		pCurrent = pNext;
	}

	//最后释放链表内存
	free(mlist);
	printf("链表内存释放!\n");
}