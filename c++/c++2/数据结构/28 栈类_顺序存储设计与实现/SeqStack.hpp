#ifndef SEQSTACK_HPP
#define SEQSTACK_HPP

#define MAX 1024

template<class T>
class SeqStack{
public:
	//构造函数
	SeqStack(){
		mSize = 0;
	}
	//压栈
	void Push(T val){
		if (this->mSize == MAX){
			return;
		}
		this->mArray[this->mSize] = val;
		this->mSize++;
	}
	//获得栈顶元素
	T Top(){
		return this->mArray[this->mSize - 1];
	}
	//弹出栈顶元素
	void Pop(){
		if (this->mSize == 0){
			return;
		}
		this->mSize--;
	}
	//栈是否为空
	bool Empty(){
		return this->mSize == 0;
	}
	//栈中元素个数
	int Size(){
		return this->mSize;
	}
public:
	int mSize;
	T mArray[MAX];
};


#endif