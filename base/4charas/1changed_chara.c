#include<stdio.h>
#include<string.h>

//如果str1和str2中出现的字符种类一样且各字符出现的次数也一样 即是互为变形词
typedef int bool;
#define true 1
#define false 0

bool is_same(char* s1, int len1, char* s2, int len2) {
  if(s1 == NULL || s2 == NULL || len1 != len2) return false;
  int map[256] = {0};
  for(int i = 0; i < len1; i++) {
	map[s1[i]]++;
	map[s2[i]]--;
  }
  for(int i = 0; i < 256; i++) {
	if(map[i] != 0) return false;
  }
  return true;
}

int main() {
  char* str1 = "123";
  char* str2 = "2231";
  bool ret = is_same(str1, strlen(str1), str2, strlen(str2)); 
  printf("ret=%d\n", ret);
  return 0;
}

