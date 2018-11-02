#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef int bool;
#define true 1
#define false 0

//12abcabca4 from=abc to=x 最终为12xxa4
//有个match变量 记录from字符串的变化
char* replace(char* str/*in-out*/, int len, char* from, int flen, char* to, int tlen) {
  if(str == NULL || len == 0 || from == NULL || flen == 0 || to == NULL || tlen == 0) return NULL;
  int flen1 = tlen;
  int match = 0;
  for(int i = 0; i < len; i++) {
	if(str[i] == from[match++]) {
	  if(match == flen) { //匹配上了 向前清0
		int tmp = i;
		while(flen-- != 0) {
		  str[tmp--] = 0;
		}
		match = 0;
 	  }
    } else {
	  match = 0; //match在什么情况下情况至关重要
	  if(str[i] == from[0]) i--; //写不出来 //12ababca4 from=abc
    }
  }
  //for(int i = 0; i < len; i++) {
  //	printf("-%c\n", str[i]);
  // }
  char* new_str = (char*) malloc (len * sizeof(char));
  //开始处理0
  int j = 0;
  for(int i = 0; i < len; i++) {
	if(str[i] != 0) {
      //printf("%c\n", str[i]);
	  new_str[j] = str[i]; j++;
    }
	if(str[i] == 0 &&(i > 0 && str[i-1] != 0)) {
	  int tmp = 0;
	  int tmp_len = flen1;
	  while(tmp_len-- != 0) {
		new_str[j++] = to[tmp++];
	  }
	}
  }
  new_str[j] = 0;
  //for(int i = 0; i < len; i++) {
//	printf("-%c\n", new_str[i]);
  //}
  return new_str; 
}

int main() {
  char str[] = "12ababcab4";
  char* from = "abc";
  char* to = "x";
  char* new_str = replace(str, strlen(str), from, strlen(from), to, strlen(to));
  printf("new_str=%s\n", new_str);
  free(new_str);
  return 0;
}
