#include"MyArray.h"

MyArray::MyArray(){

	//默认的数组容量
	int capacity = 20;
	this->pAddress = new int[capacity];
	this->mCapacity = capacity;
	this->mSize = 0;

}
MyArray::MyArray(int capacity){
	if (capacity < 0){
		capacity = 20;
	}
	this->pAddress = new int[capacity];
	this->mCapacity = capacity;
	this->mSize = 0;
}
//根据位置添加元素
void MyArray::SetData(int pos, int val){
	if (pos < 0 || pos > this->mCapacity - 1){
		return;
	}

	this->pAddress[pos] = val;
}
int MyArray::GetData(int pos){
	return this->pAddress[pos];
}
//尾插法
void MyArray::PushBack(int val){
	if (this->mSize == this->mCapacity){
		return;
	}
	this->pAddress[this->mSize] = val;
	this->mSize++;
}
//尾部删除
void MyArray::PopBack(){
	if (this->mSize <= 0){
		return;
	}
	this->mSize--;
}
//指定位置删除
void MyArray::RemoveByPos(int pos){
	if (pos < 0 || pos > this->mSize - 1){
		return;
	}
	for (int i = pos; i < this->mSize - 1; i ++){
		this->pAddress[i] = this->pAddress[i + 1];
	}
	this->mSize--;
}
//获得容量
int MyArray::GetCapacity(){
	return this->mCapacity;
}
//获得大小
int MyArray::GetSize(){
	return this->mSize;
}
//析构函数
MyArray::~MyArray(){
	if (this->pAddress != NULL){
		delete[] this->pAddress;
		this->pAddress = NULL;
		this->mCapacity = 0;
		this->mSize = 0;
	}
}