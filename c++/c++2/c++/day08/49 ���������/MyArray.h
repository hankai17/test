#ifndef MYARRAY_H
#define MYARRAY_H

class MyArray{

public:
	//无参构造函数，用户没有指定容量，则初始化为100
	MyArray();
	//有参构造函数，用户指定容量初始化
	MyArray(int capacity);

	//用户操作接口

	//根据位置添加元素
	void SetData(int pos, int val);
	//获得指定位置数据
	int GetData(int pos);
	//尾插法
	void PushBack(int val);
	//获得长度
	int GetLength();
	//析构函数，释放数组空间

	//-----------重载[]操作符 start-------
	int& operator[](int index){
		return pAdress[index];
	}
	//-----------重载[]操作符 end-------

	~MyArray();

private:
	int mCapacity; //数组一共可容纳多少个元素
	int mSize; //当前有多少个元素
	int* pAdress; //指向存储数据的空间

};


#endif