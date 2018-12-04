#ifndef SEQQUEUE_HPP
#define SEQQUEUE_HPP

#define MAX 1024

template<class T>
class SeqQueue{
public:
	//构造函数
	SeqQueue(){
		mSize = 0;
	}

	//入队
	void Push(T val){
		if (this->mSize == MAX){
			return;
		}
		this->mArray[this->mSize] = val;
		this->mSize++;
	}
	//出队
	void Pop(){
		if (this->mSize == 0){
			return;
		}
		for (int i = 0; i < this->mSize - 1; i ++){
			this->mArray[i] = this->mArray[i + 1];
		}
		this->mSize--;
	}

	//获得队头元素
	T Front(){
		return this->mArray[0];
	}
	//获得队尾元素
	T Back(){
		return this->mArray[this->mSize - 1];
	}
	//大小
	int Size(){
		return mSize;
	}

public:
	T mArray[MAX];
	int mSize;
};


#endif