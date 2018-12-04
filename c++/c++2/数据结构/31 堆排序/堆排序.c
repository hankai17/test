#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX 20

void printArray(int arr[], int len){
	for (int i = 0; i < len; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void MySwap(int arr[],int i,int j){

	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;

}

//调整堆
void HeapAdjust(int arr[],int index, int len){

	int lchild = index * 2 + 1;
	int rchild = index * 2 + 2;
	int max = index;

	if (index < len / 2){
	
		if (lchild < len && arr[lchild] > arr[max]){
			max = lchild;
		}

		if (rchild < len && arr[rchild] > arr[max]){
			max = rchild;
		}

		if (max != index){
			MySwap(arr, index, max);
			HeapAdjust(arr,max,len);
		}

	}
}

//堆调整
void HeapAdjust2(int arr[], int index, int len){

	//左子树下标
	int lchild = index * 2 + 1;
	//右子树下标
	int rchild = index * 2 + 2;

	int max = index;

	while (lchild < len || rchild < len){

		if (lchild < len && arr[lchild] > arr[max]){
			max = lchild;
		}

		if (rchild < len && arr[rchild] > arr[max]){
			max = rchild;
		}

		if (max != index){
			MySwap(arr, max, index);
			index = max; //index表示当前要调整的位置
			lchild = max * 2 + 1; //当前调整位置的左节点
			rchild = max * 2 + 2; //当前调整位置的右节点
		}
		else{
			break;
		}

	}

}

void HeapSort(int arr[],int len){

	//初始化堆
	for (int i = len / 2 - 1; i >= 0; i --){
		//调整堆
		HeapAdjust(arr,i,len);
	}


	for (int i = len - 1; i >= 0; i --){
		//交换堆顶和末尾元素
		MySwap(arr, 0, i);
		//调整堆
		HeapAdjust(arr,0,i);
	}

}


int main(){

	int arr[MAX];
	srand((unsigned int)time(NULL));
	for (int i = 0; i < MAX; i++){
		arr[i] = rand() % MAX;
	}

	//排序前
	printArray(arr, MAX);
	//堆排序
	HeapSort(arr, MAX);
	//排序后
	printArray(arr, MAX);


	system("pause");
	return EXIT_SUCCESS;
}