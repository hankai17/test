#ifndef MYARRAY_H
#define MYARRAY_H

#include<stdlib.h>

class MyArray{
public:
	MyArray();
	MyArray(int capacity);
	//根据位置添加元素
	void SetData(int pos,int val);
	int GetData(int pos);
	//尾插法
	void PushBack(int val);
	//尾部删除
	void PopBack();
	//指定位置删除
	void RemoveByPos(int pos);
	//获得容量
	int GetCapacity();
	//获得大小
	int GetSize();

	//写个函数 告诉编译 mArray[0] 要做什么？
	int& operator[](int index){
		return this->pAddress[index];
	}


	//析构函数
	~MyArray();
private:
	int* pAddress; //数组的首地址
	int mCapacity; //容量，表示这个数组一共可以容纳多少个元素
	int mSize; //表示当前数组有多少个元素
};




#endif