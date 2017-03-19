#ifndef LINKSTACK_H
#define LINKSTACK_H

template<class T>
class StackNode{
public:
	T data;
	StackNode<T>* next;
};

template<class T>
class LinkStack{
public:
	LinkStack(){
		mHeader.next = NULL;
		mSize = 0;
	}

	void Push(T val){
		
		//创建新节点
		StackNode<T>* newnode = new StackNode<T>;
		newnode->data = val;
		newnode->next = NULL;

		//新节点入链表
		newnode->next = mHeader.next;
		mHeader.next = newnode;
		mSize++;
	}

	//弹出栈顶元素
	void Pop(){
		if (this->mSize == 0){
			return;
		}
		//缓存待删除元素
		StackNode<T>* pDel = mHeader.next;
		mHeader.next = pDel->next;
		delete pDel;
		mSize--;
	}

	//获得栈顶元素
	T Top(){
		return mHeader.next->data;
	}

	//栈是否为空
	bool Empty(){
		return mSize == 0;
	}

	//栈中元素个数
	int Size(){
		return mSize;
	}

	//析构函数
	~LinkStack(){
		StackNode<T>* pCurrent = mHeader.next;
		while (pCurrent != NULL){
			//缓存下一个节点
			StackNode<T>* pNext = pCurrent->next;
			delete pCurrent;
			pCurrent = pNext;
		}
	}

public:
	StackNode<T> mHeader; //头结点
	int mSize;
};



#endif