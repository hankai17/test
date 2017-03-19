#include"LinkList.h"

//初始化
LinkList Init_LinkList(){
	LList* list = (LList*)malloc(sizeof(LList));
	if (list == NULL){
		return NULL;
	}
	list->header.next = NULL;
	list->size = 0;

	return list;
}
//指定位置插入
void Insert_LinkList(LinkList list, int pos, LinkNode* data){
	if (list == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	if (pos < 0 || pos > mlist->size){
		pos = mlist->size;
	}
	if (data == NULL){
		return;
	}

#if 0
	//增加以下操作降低插入效率，由用户自觉插入非重复元素
	//判断指针地址是否重复 
	LinkNode* pTest = mlist->header.next;
	while (pTest != NULL){
		if (pTest == data){
			printf("指针重复!\n");
			return;
		}
		pTest = pTest->next;
	}
#endif

	//辅助指针变量
	LinkNode* pCurrent = &(mlist->header);
	int i = 0;
	for (; i < pos;i++){
		pCurrent = pCurrent->next;
	}

	//新数据入链表
	data->next = pCurrent->next;
	pCurrent->next = data;
	mlist->size++;
}

//头部插入操作
void PushFront_LinkList(LinkList list, LinkNode* data){
	if (list == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	Insert_LinkList(list, 0, data);
}
//尾部插入操作
void PushBack_LinkList(LinkList list, LinkNode* data){
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
	if (pos < 0 || pos > mlist->size - 1){
		return;
	}
	if (mlist->size == 0){
		return;
	}
	
	//辅助指针变量
	LinkNode* pCurernt = &(mlist->header);
	int i = 0;
	for (; i < pos;i++){
		pCurernt = pCurernt->next;
	}

	//缓存下被删除节点
	LinkNode* pDel = pCurernt->next;
	//待删除节点前驱节点的后继指针指向待删除节点的后继节点
	pCurernt->next = pDel->next;
	mlist->size--;
}
//头部位置删除
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
//尾部位置删除
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
//根据值删除
void RemoveByVal_LinkList(LinkList list, LinkNode* data, DATA_COMPARE compare){
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

	//辅助指针变量
	LinkNode* pPrev = &(mlist->header);
	LinkNode* pCurrent = pPrev->next;
	while (pCurrent != NULL){
		if (compare(pCurrent,data)){
			pPrev->next = pCurrent->next;
			mlist->size--;
			break;
		}

		pPrev = pCurrent;
		pCurrent = pPrev->next;
	}
}
//链表大小
int Size_LinkList(LinkList list){
	if (list == NULL){
		return -1;
	}
	LList* mlist = (LList*)list;
	return mlist->size;
}
//打印链表
void Print_LinkList(LinkList list, DATA_PRINT print){
	if (list == NULL){
		return;
	}
	if (print == NULL){
		return;
	}
	LList* mlist = (LList*)list;
	//辅助指针变量
	LinkNode* pCurrent = mlist->header.next;
	while (pCurrent != NULL){
		print(pCurrent);
		pCurrent = pCurrent->next;
	}
}
//销毁链表
void Destroy_LinkList(LinkList list){
	if (list == NULL){
		return;
	}
	free(list);
}