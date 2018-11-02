#include<stdio.h>
#include<string.h>
typedef int bool;
#define true 1
#define false 0

//str="a0000b000" k=3 返回"a0000b"

//需要两个变量start 0开始的位置 count 0目前连续的个数
char* remove_zero(char* str, int len, int k) {
  if(str == NULL || len == 0 || k < 1) return str;
  //char* start = NULL;
  int start = -1;
  int count = 0;
  for(int i = 0; i < len; i++) {
	if(str[i] == '0') {
	  start = start == -1 ? i : start;
	  count++;
 	  //if(count == k) { //开始拼接
	  //}
	} else {
	  if(count == k) { //在这里开始拼接 而非上面 为什么
	    while(count-- != 0) {
		  str[start++] = 0;
		}
	  }
	  start = -1;
	  count = 0;
    }
  }
  if(count == k) {
	while(count-- != 0)
	  str[start++] = 0;
  }
 //这道题有问题吧 
}

int main() {
  return 0;
}
