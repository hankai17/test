#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef int bool;
#define true 1
#define false 0

//2147483647不会溢出 2147483648会溢出
//首先拆分成两个函数 is_valid 跟处理函数

bool is_valid(char* str, int len) {
  if(str == NULL || len == 0) return false;
  if(str[0] != '-' && (str[0] < '0' || str[0] > '9')) return false; //不以-开头 不以数字开头
  if(str[0] == '0' && len > 1) return false; //不以0开头
  if(str[0] == '-' && (str[1] == '0' || len == 1)) return fasle;
  for(int i = 1; i < len; i++) {
	if(str[i] < '0' || str[i] > '9') return false;
  }
  return true;
}

//这一题考的是溢出 看不懂

int main() {
  return 0;
}
