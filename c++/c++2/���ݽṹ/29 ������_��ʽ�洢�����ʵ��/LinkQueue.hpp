#ifndef LINKQUEUE_HPP
#define LINKQUEUE_HPP

template<class T>
class QueueNode{
public:
	T data;
	QueueNode<T>* next;
};

template<class T>
class LinkQueue{
public:
	LinkQueue(){
		mHeader.next = NULL;
		mSize = 0;
	}
	//入队
	void Push(T val){
		//创建新节点
		QueueNode<T>* newnode = new QueueNode<T>;
		newnode->data = val;
		newnode->next = NULL;

		//辅助指针
		QueueNode<T>* pCurrent = &(this->mHeader);
		for (int i = 0; i < this->mSize; i++){
			pCurrent = pCurrent->next;
		}

		//新节点入链表 头作为队头
		newnode->next = pCurrent->next;
		pCurrent->next = newnode;
		mSize++;
	}
	//出队
	void Pop(){
		if (this->mSize == 0){
			return;
		}	
		//待删除节点
		QueueNode<T>* pDel = mHeader.next;
		mHeader.next = pDel->next;
		delete pDel;
		this->mSize--;
	}
	//获得队头元素
	T Front(){
		return mHeader.next->data;
	}
	//获得队尾元素
	T Back(){
		//辅助指针
		QueueNode<T>* pCurrent = &(this->mHeader);
		for (int i = 0; i < this->mSize; i++){
			pCurrent = pCurrent->next;
		}

		return pCurrent->data;
	}
	//获得队列长度
	int Size(){
		return this->mSize;
	}
	//析构函数
	~LinkQueue(){
		QueueNode<T>* pCurrent = this->mHeader.next;
		while (pCurrent != NULL){
			QueueNode<T>* pNext = pCurrent->next;
			delete pCurrent;
			pCurrent = pNext;
		}
	}
public:
	QueueNode<T> mHeader;
	int mSize;
};



#endif