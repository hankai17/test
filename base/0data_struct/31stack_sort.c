#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAX 20

//数组就是堆

void print_array(int arr[], int len) {
  for (int i = 0; i < len; i++) {
	printf("%d ", arr[i]);
  }
  printf("\n");
}

void my_swap(int arr[],int i, int j) {
  int tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}

//只需要一个额外变量max记录父子三个那个最大的下标
void head_adjust(int arr[], int index, int len) { //index是父节点下标
  int lchild = index * 2 + 1;
  int rchild = index * 2 + 2;
  int max = index;
  if (index < len / 2) {
	if (lchild < len && arr[lchild] > arr[max]) {
	  max = lchild;
	}
	if (rchild < len && arr[rchild] > arr[max]) {
	  max = rchild;
	}
	if (max != index) { //交换父子位置
	  my_swap(arr, index, max);
	  head_adjust(arr, max, len); //感觉有点多余啊
	}
  }
}

//堆调整非递归
void head_adjust2(int arr[], int index, int len){
  int lchild = index * 2 + 1;
  int rchild = index * 2 + 2;
  int max = index;

  while (lchild < len || rchild < len) {
	if (lchild < len && arr[lchild] > arr[max]){
	  max = lchild;
	}
	if (rchild < len && arr[rchild] > arr[max]){
	  max = rchild;
	}
	if (max != index) {
	  my_swap(arr, max, index);
	  index = max; //index表示当前要调整的位置
	  lchild = max * 2 + 1; //当前调整位置的左节点
	  rchild = max * 2 + 2; //当前调整位置的右节点
	} else {
	  break;
	}
  }
}

void heap_sort(int arr[],int len){
  for (int i = len / 2 - 1; i >= 0; i--){ //堆从下到上依次 获取父节点下标i
	head_adjust(arr, i, len); //调整堆
  }
  //目前堆顶是一个最大值
  for (int i = len - 1; i >= 0; i--) {
	my_swap(arr, 0, i); //交换堆顶和末尾元素
	head_adjust(arr, 0, i); //调整堆
  }
}


int main() {
  int arr[MAX];
  srand((unsigned int)time(NULL));
  for (int i = 0; i < MAX; i++){
	arr[i] = rand() % MAX;
  }
  print_array(arr, MAX); //排序前
  heap_sort(arr, MAX); //堆排序
  print_array(arr, MAX); //排序后
  return 0;
}
