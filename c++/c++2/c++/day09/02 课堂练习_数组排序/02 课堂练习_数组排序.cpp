#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;


template<class T>
void MyPrint(T arr[], int len){
	for (int i = 0; i < len; i ++){
		cout << arr[i] << " ";
	}
	cout << endl;
}

template<class T>
void MySort(T arr[],int len){

	for (int i = 0; i < len - 1; i ++){
		for (int j = 0; j < len - i - 1; j++){
			if (arr[j] < arr[j+1]){
				T temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
		}
	}

}

int main(){

	int intArr[] = { 1, 6, 3, 8, 2 };
	int len = sizeof(intArr) / sizeof(int);

	double doubleArr[] = { 7.86, 4, 56, 1.23, 9.99 };
	int doubleLen = sizeof(doubleArr) / sizeof(double);

	MyPrint(intArr,len);
	MySort(intArr, len);
	MyPrint(intArr, len);

	MyPrint(doubleArr, doubleLen);
	MySort(doubleArr, doubleLen);
	MyPrint(doubleArr, doubleLen);

	system("pause");
	return EXIT_SUCCESS;
}