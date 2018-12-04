#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include<stdlib.h>
#include<string.h>

//动态数组结构体
typedef struct _DYNAMICARRAY{
	void** address;
	int capacity;
	int size;
}DArray;

typedef void* DynamicArray;
//比较函数指针
typedef int(DATACOMPARE)(void*,void*);
//打印函数指针
typedef void(DATAPRINT)(void*);

//初始化数组
DynamicArray Init_DynamicArray();
//指定位置插入
void Insert_DynamicArray(DynamicArray darray,int pos, void* data);
//尾部插入操作
void PushBack_DynamicArray(DynamicArray darray, void* data);
//头部插入操作
void PushFront_DynamicArray(DynamicArray darray, void* data);
//指定位置删除
void RemoveByPos_DynamicArray(DynamicArray darray, int pos);
//尾部删除操作
void PopBack_DynamicArray(DynamicArray darray);
//头部删除操作
void PopFront_DynamicArray(DynamicArray darray);
//值删除
void RemoveByVal_DynamicArray(DynamicArray darray, void* data, DATACOMPARE compare);
//获得指定位置元素
void* Get_DynamicArray(DynamicArray darray,int pos);
//数组大小
int Size_DynamicArray(DynamicArray darray);
//数组容量
int Capacity_DynamicArray(DynamicArray darray);
//遍历
void Print_DynamicArray(DynamicArray darray,DATAPRINT print);
//销毁数组
void Destroy_DynamicArray(DynamicArray darray);


#endif