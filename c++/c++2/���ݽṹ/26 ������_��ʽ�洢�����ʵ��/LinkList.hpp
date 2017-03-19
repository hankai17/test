#ifndef LINKLIST_HPP
#define LINKLIST_HPP

//链表节点
template<class T>
class LinkNode{
public:
	T data;
	LinkNode<T>* next;
};

//链表
template<class T>
class LinkList{
public:
	//初始化
	LinkList(){
		this->mHeader.next = NULL;
		this->mSize = 0;
	}
	//指定位置插入
	void Insert(int pos,T val){
		if (pos < 0 || pos > this->mSize){
			pos = this->mSize;
		}

		//创建新的节点
		LinkNode<T>* newnode = new LinkNode<T>;
		newnode->data = val;
		newnode->next = NULL;

		//新节点入链表
		LinkNode<T>* pCurrent = &(this->mHeader);
		for (int i = 0; i < pos;i++){
			pCurrent = pCurrent->next;
		}

		newnode->next = pCurrent->next;
		pCurrent->next = newnode;
		this->mSize++;
	}
	//头部插入
	void Push_Front(T val){
		Insert(0, val);
	}
	//尾部插入
	void Push_Back(T val){
		Insert(this->mSize, val);
	}
	//指定位置删除
	void RemoveByPos(int pos){

		if (this->mSize == 0){
			return;
		}
		if (pos < 0 || pos > this->mSize - 1){
			return;
		}

		//找到被删除节点的前一个节点
		LinkNode<T>* pCurrent = &(this->mHeader);
		for (int i = 0; i < pos;i ++){
			pCurrent = pCurrent->next;
		}

		//缓存被删除节点
		LinkNode<T>* pDel = pCurrent->next;
		//重新链接被删除节点的前驱和后继节点
		pCurrent->next = pDel->next;
		//释放被删除节点内存
		delete pDel;
		this->mSize--;
	}
	//头删
	void Pop_Front(){
		if (this->mSize == 0){
			return;
		}
		RemoveByPos(0);
	}
	//尾删
	void Pop_Back(){
		if (this->mSize == 0){
			return;
		}
		RemoveByPos(this->mSize-1);
	}
	//获得链表大小
	int Size(){
		return this->mSize;
	}
	//值删除
	void RemoveByVal(T val){

		if (this->mSize == 0){
			return;
		}

		//辅助指针变量
		LinkNode<T>* pPrev = &(this->mHeader);
		LinkNode<T>* pCurrent = pPrev->next;
		while (pCurrent != NULL){
			
			if (pCurrent->data == val){
				pPrev->next = pCurrent->next;
				delete pCurrent;
				this->mSize--;
				break;
			}

			pPrev = pCurrent;
			pCurrent = pPrev->next;
		}
	}

	//打印链表
	template<class MyPrint>
	void Print(MyPrint print){
		LinkNode<T>* pCurrent = this->mHeader.next;
		while (pCurrent != NULL){
			print(pCurrent->data);
			pCurrent = pCurrent->next;
		}
	}

	//析构函数
	~LinkList(){
		LinkNode<T>* pCurrent = this->mHeader.next;
		while (pCurrent != NULL){
			LinkNode<T>* pNext = pCurrent->next;
			delete pCurrent;
			pCurrent = pNext;
		}
	}
	
public:
	LinkNode<T> mHeader;
	int mSize;
};


#endif