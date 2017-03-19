#pragma once


template<class T>
class MyArray{
public:
	MyArray(int capacity){
		this->mCapacity = capacity;
		this->mSize = 0;
		this->pAddress = new T[this->mCapacity];
	}

	//拷贝构造
	MyArray(const MyArray& arr){
		this->mCapacity = arr.mCapacity;
		this->mSize = arr.mSize;
		this->pAddress = new T[this->mCapacity];
		for (int i = 0; i < this->mSize; i ++){
			this->pAddress[i] = arr.pAddress[i];
		}
	}
	//重载=号操作符
	MyArray& operator=(const MyArray& arr){
	
		if (this->pAddress != NULL){
			delete[] this->pAddress;
			this->pAddress = NULL;
		}

		this->mCapacity = arr.mCapacity;
		this->mSize = arr.mSize;
		this->pAddress = new T[this->mCapacity];
		for (int i = 0; i < this->mSize; i++){
			this->pAddress[i] = arr.pAddress[i];
		}

		return *this;
	}
	//重载[]操作符
	T& operator[](int index){
		return this->pAddress[index];
	}

	//尾插法
	void PushBack(T val){
		if (this->mSize == this->mCapacity){
			return;
		}
		this->pAddress[this->mSize] = val;
		this->mSize++;
	}

	//尾删法
	void PopBack(){
		this->mSize--;
	}

	~MyArray(){
		if (this->pAddress != NULL){
			delete[] this->pAddress;
			this->pAddress = NULL;
			this->mCapacity = 0;
			this->mSize = 0;
		}
	}


private:
	T* pAddress; //指向数组的指针
	int mCapacity; //容量
	int mSize; //当前有多少个元素
};