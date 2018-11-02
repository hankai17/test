#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef int bool;
#define true 1
#define false 0
//12345的旋转词有"12345","23451","34512","45123"和"51234"
//b2 = b1 + b1 设b1长度为N 在b2中任意长度为N的字串都是b1的旋转词

bool get_index_of(char* str1, int len1, char* str2, int len2) {
  return false;
}

bool is_rotate(char* str, int len, char* k, int len1) {
  if(str == NULL || k == NULL || len == 0 || len1 != len) return false;
  int b2_len = 2 * len;
  //char b2[b2_len] = {0};
  char* b2 = (char*) malloc (b2_len * sizeof(char));
  memcpy(b2, str, len);
  strncat(b2, str, len);
  return get_index_of(b2, b2_len, k, len1);
}

int main() {
  char* str = "1ab2";
  char* k = "ab12";
  bool ret = is_rotate(str, strlen(str), k, strlen(k));
  printf("ret=%d\n", ret);
  return 0;
}
