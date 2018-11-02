#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define MAX 20

//������Ƕ�

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

//ֻ��Ҫһ���������max��¼���������Ǹ������±�
void head_adjust(int arr[], int index, int len) { //index�Ǹ��ڵ��±�
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
	if (max != index) { //��������λ��
	  my_swap(arr, index, max);
	  head_adjust(arr, max, len); //�о��е���డ
	}
  }
}

//�ѵ����ǵݹ�
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
	  index = max; //index��ʾ��ǰҪ������λ��
	  lchild = max * 2 + 1; //��ǰ����λ�õ���ڵ�
	  rchild = max * 2 + 2; //��ǰ����λ�õ��ҽڵ�
	} else {
	  break;
	}
  }
}

void heap_sort(int arr[],int len){
  for (int i = len / 2 - 1; i >= 0; i--){ //�Ѵ��µ������� ��ȡ���ڵ��±�i
	head_adjust(arr, i, len); //������
  }
  //Ŀǰ�Ѷ���һ�����ֵ
  for (int i = len - 1; i >= 0; i--) {
	my_swap(arr, 0, i); //�����Ѷ���ĩβԪ��
	head_adjust(arr, 0, i); //������
  }
}


int main() {
  int arr[MAX];
  srand((unsigned int)time(NULL));
  for (int i = 0; i < MAX; i++){
	arr[i] = rand() % MAX;
  }
  print_array(arr, MAX); //����ǰ
  heap_sort(arr, MAX); //������
  print_array(arr, MAX); //�����
  return 0;
}
