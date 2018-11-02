#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX 10

void printArray(int arr[], int len){
	for (int i = 0; i < len; i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
}

//²åÈëÅÅÐò
void InsertSort(int arr[],int len){
	
	for (int i = 1; i < len; i ++){
		
		if (arr[i] < arr[i - 1]){
			
			int temp = arr[i];
			int j = i - 1;
			for (; j >= 0 && temp < arr[j]; j --){
				arr[j + 1] = arr[j];
			}
			arr[j + 1] = temp;

		}

	}

}


int main(){

	int arr[MAX];
	srand((unsigned int)time(NULL));
	for (int i = 0; i < MAX; i++){
		arr[i] = rand() % MAX;
	}

	//ÅÅÐòÇ°
	printArray(arr, MAX);
	//Ã°ÅÝÅÅÐò
	InsertSort(arr, MAX);
	//ÅÅÐòºó
	printArray(arr, MAX);


	system("pause");
	return EXIT_SUCCESS;
}