#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef int bool;
#define true 1
#define false 0

//反转字符串 mike love dog 变成dog love mike

void reverse_some(char* str, int len, int f, int t) { //in-out
  if(str == NULL || len == 0 || f < 0 || t > len) return;
  char tmp;
  while(f < t) {
	tmp = str[f];
	str[f] = str[t];
    str[t] = tmp;
    f++;
    t--;
  }
}

char* reverse(char* str, int len) {
  if(str == NULL || len == 0) return NULL;
  reverse_some(str, len, 0, len-1);
  printf("%s\n", str);
  int l = -1;
  int r = -1;
  for(int i = 0; i < len; i++) {
	if(str[i] != ' ') {
	  l = (i == 0 || str[i-1] == ' ') ?  i : l;
	  r = (i == len - 1 || str[i+1] == ' ') ?  i : r;
    }
    if(l != -1 && r != -1) {
	  reverse_some(str, len, l, r);
	  l = -1;
	  r = -1;
    }
  }
  return str;
}
int main() {
  char str[] = "mike love dog";
  char* ret = reverse(str, strlen(str)); 
  printf("%s\n", ret);
  return 0;
}
