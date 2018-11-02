#include<iostream>
#include<stdio.h>
#include<deque>

#define ARRAY_SIZE 8
#define WIN_SIZE 3

void get_win_max(int* a) {
  std::deque<int> qmax; //存储下标 且必须保证队首是最大值
  std::deque<int> result;

  for(int i = 0; i < ARRAY_SIZE; i++) {
	if(!qmax.empty()) {
	  if(a[i] <= a[qmax.back()]) { //win内值小3 则push下标
		qmax.push_back(i);
		//窗口满则弹出队首
		while(!qmax.empty() && (i - qmax.front() + 1 > WIN_SIZE) ) { //while(i - qmax.front() + 1 > WIN_SIZE && !qmax.empty())
		  qmax.pop_front();
		}
	  } else { //win内出现一个大值5  则依次弹出qmax 直到5放到到合适位置处为止
		while(!qmax.empty() && a[i] > a[qmax.back()]) { //while(a[i] > a[qmax.back()] && !qmax.empty())
		  qmax.pop_back();
		}
		qmax.push_back(i);
	  }
	} else {
	  qmax.push_back(i);
	}

	if(i >= WIN_SIZE -1) {
	  int tmp = qmax.front();
	  result.push_back(tmp);
	}
  }
  while(!result.empty()) {
	std::cout<<a[result.front()]<<std::endl;
	result.pop_front();
  }
  return;
}

int main() {
  int array[ARRAY_SIZE] = {4, 3, 5, 4, 3, 3, 6, 7 };
  get_win_max(array);
  return 0;
}

