#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define MAX 10

void printArray(int arr[], int len){
	for (int i = 0; i < len; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
}

//快速排序
void QuickSort(int arr[],int start, int end){

	int i = start;
	int j = end;
	//取基准数
	int target = arr[start];
	if (i < j){
		
		while (i < j){
			
			//从右向左找比基准数大的
			while (i < j && arr[j] < target){
				j--;
			}
			if (i < j){
				arr[i] = arr[j];
				i++;
			}

			//从左向右找比基准数小的
			while (i < j && arr[i] > target){
				i++;
			}
			if (i < j){
				arr[j] = arr[i];
				j--;
			}
		}
		
		//i的位置就是基准数最合适的位置
		arr[i] = target;
		//快速排序左半部分
		QuickSort(arr,start,i - 1);
		//快速排序有半部分
		QuickSort(arr, i + 1, end);

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
	//冒泡排序
	QuickSort(arr, 0,MAX - 1);
	//排序后
	printArray(arr, MAX);

	system("pause");
	return EXIT_SUCCESS;
}