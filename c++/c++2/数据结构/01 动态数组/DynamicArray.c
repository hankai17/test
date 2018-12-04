#include "DynamicArray.h"

//初始化数组
DynamicArray Init_DynamicArray(){

	DArray* darray = malloc(sizeof(DArray));
	if (darray == NULL){
		return NULL;
	}
	darray->capacity = 5; //初始化容量5
	darray->size = 0;
	darray->address = (void**)malloc(sizeof(void*) * darray->capacity);

	return darray;
}
//指定位置插入
void Insert_DynamicArray(DynamicArray darray, int pos, void* data){
	if (darray == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	DArray* arr = (DArray*)darray;
	//判断空间是否足够
	if (arr->size == arr->capacity){
		
		int newcapacity = arr->capacity * 2; //两倍空间扩展
		//申请空间
		void** newspace = (void**)malloc(sizeof(void*) * newcapacity);
		//拷贝原空间数据到新空间
		memcpy(newspace,arr->address,sizeof(void*) * arr->capacity);
		//释放旧空间
		free(arr->address);
		arr->address = newspace;
		arr->capacity = newcapacity;

	}

	//判断位置是否有效，如果pos越界，调整为在尾部插入
	if (pos < 0 || pos > arr->size){
		pos = arr->size;
	}
	//移动元素
	int i = arr->size - 1;
	for (; i >= pos;i--){
		arr->address[i + 1] = arr->address[i];
	}
	arr->address[pos] = data;
	arr->size++;
}
//尾部插入操作
void PushBack_DynamicArray(DynamicArray darray, void* data){
	if (darray == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	DArray* arr = (DArray*)darray;
	Insert_DynamicArray(darray, arr->size, data);
}
//头部插入操作
void PushFront_DynamicArray(DynamicArray darray, void* data){
	if (darray == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	DArray* arr = (DArray*)darray;
	Insert_DynamicArray(darray, 0, data);
}
//位置删除
void RemoveByPos_DynamicArray(DynamicArray darray, int pos){
	if (darray == NULL){
		return;
	}
	DArray* arr = (DArray*)darray;
	//数组是否为空
	if (arr->size == 0){
		return;
	}
	if (pos < 0 || pos > arr->size - 1){
		return;
	}
	//删除元素
	int i = pos;
	for (; i < arr->size - 1; i++){
		arr->address[i] = arr->address[i + 1];
	}

	arr->size--;
}
//尾部删除操作
void PopBack_DynamicArray(DynamicArray darray){
	if (darray == NULL){
		return;
	}
	DArray* arr = (DArray*)darray;
	//数组是否为空
	if (arr->size == 0){
		return;
	}
	RemoveByPos_DynamicArray(darray, arr->size - 1);
}
//头部删除操作
void PopFront_DynamicArray(DynamicArray darray){
	if (darray == NULL){
		return;
	}
	DArray* arr = (DArray*)darray;
	//数组是否为空
	if (arr->size == 0){
		return;
	}
	RemoveByPos_DynamicArray(darray, 0);
}
//值删除
void RemoveByVal_DynamicArray(DynamicArray darray, void* data, DATACOMPARE compare){
	if (darray == NULL){
		return;
	}
	if (data == NULL){
		return;
	}
	DArray* arr = (DArray*)darray;
	//数组是否为空
	if (arr->size == 0){
		return;
	}
	//查找删除元素
	int i = 0;
	for (; i < arr->size;i++){
		if (compare(arr->address[i], data)){
			RemoveByPos_DynamicArray(darray, i);
			break;
		}
	}
}

//获得指定位置元素
void* Get_DynamicArray(DynamicArray darray, int pos){
	if (darray == NULL){
		return NULL;
	}
	DArray* arr = (DArray*)darray;
	if (pos < 0 || pos >= arr->size){
		return NULL;
	}
	return arr->address[pos];
}

//数组大小
int Size_DynamicArray(DynamicArray darray){
	if (darray == NULL){
		return -1;
	}
	DArray* arr = (DArray*)darray;
	return arr->size;
}
//数组容量
int Capacity_DynamicArray(DynamicArray darray){
	if (darray == NULL){
		return -1;
	}
	DArray* arr = (DArray*)darray;
	return arr->capacity;
}
//遍历
void Print_DynamicArray(DynamicArray darray, DATAPRINT print){
	if (darray == NULL){
		return;
	}
	DArray* arr = (DArray*)darray;
	int i = 0;
	for (; i < arr->size;i++){
		print(arr->address[i]);
	}
}
//销毁数组
void Destroy_DynamicArray(DynamicArray darray){
	if (darray == NULL){
		return;
	}
	free(darray);
}