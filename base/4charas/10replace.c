#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef int bool;
#define true 1
#define false 0

//a b  c转换成a%20b%20%20c  即把空格换成%20

char* replace(char* str, int len) {
  if(str == NULL || len == 0) return NULL;
  int b_num = 0;
  for(int i = 0; i < len; i++) {
	if(str[i] == ' ') {
	  b_num++;
	}
  }
  int j = len + b_num * 2; //j即新字符串总长度
  char* n_str = (char*) malloc ((j+1) * sizeof(char));
  memset(n_str, 0, j+1);
  int k = j - 1;
  //判断
  //for(int i = len - 1; i != 0; i--) {
  for(int i = len - 1; i > -1; i--) {
	if(str[i] != ' ') {
	  n_str[k--] = str[i];
	} else {
	  n_str[k--] = '0';
	  n_str[k--] = '2';
	  n_str[k--] = '%';
	}
  }
  return n_str;
}
int main() {
  char* str = "a b  c";
  char* ret = replace(str, strlen(str)); 
  printf("%s\n", ret);
  return 0;
}
