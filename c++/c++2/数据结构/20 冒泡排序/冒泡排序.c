#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX 10

void printArray(int arr[],int len){
	for (int i = 0; i < len;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}

//交换两个元素
void MySwap(int arr[],int i, int j){
	int temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}

//冒泡排序
void BubbleSort(int arr[], int len){

	for (int i = 0; i < len; i ++){
		for (int j = len - 1; j > i;j--){
			if (arr[j] < arr[j-1]){
				MySwap(arr,j,j-1);
			}
		}
	}

}

//冒泡排序改进
void BubbleSort2(int arr[],int len){
	
	int flag = 0; //没有排好
	for (int i = 0; i < len && flag == 0; i++){
		flag = 1; //认为已经排好
		for (int j = len - 1; j > i; j --){
			if (arr[j] < arr[j-1]){
				flag = 0; //没有排好 再拍一次
				MySwap(arr,j,j-1);
			}
		}
	}

}

int main(){

	int arr[MAX];
	srand((unsigned int)time(NULL));
	for (int i = 0; i < MAX; i++){
		arr[i] = rand() % MAX;
	}

	//排序前
	printArray(arr,MAX);
	//冒泡排序
	//BubbleSort(arr,MAX);
	BubbleSort2(arr, MAX);
	//排序后
	printArray(arr, MAX);


	system("pause");
	return EXIT_SUCCESS;
}