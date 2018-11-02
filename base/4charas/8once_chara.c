#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef int bool;
#define true 1
#define false 0

bool is_once(char* str, int len) {
  if(str == NULL || len == 0) return false;
  bool map[256] = {false};
  for(int i = 0; i < len; i++) {
	if(map[str[i]]) return false;
	map[str[i]] = true;
  }
  return true;
}

int main() {
  //char* str = "121";
  char* str = "123";
  bool ret = is_once(str, strlen(str));
  printf("ret=%d\n", ret);
  return 0;
}

//另一种方法是堆排序 后判断是否有重复点
