#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef int bool;
#define true 1
#define false 0

//aaabbadddffc  a3b2a1d3f2c1

char* statics_chara(char* str, int len) {
  if(str == NULL || len == 0) return NULL;
  int num = 1; 
  char* ret = (char*) malloc (256 * sizeof(char));
  memset(ret, 0, sizeof(ret));
  strcat(ret, str[0]);
  char str_num[1] = {0};
  for(int i = 1; i < len; i++) { //从第二个开始
	if(str[i] != str[i-1]) {
	  strcat(ret, itoa(num, str_num, 10)); 
	  char tmp[2] = {0};
	  tmp[0] = str[i]
	  strncat(ret, tmp, 2);
	} else {
	  num++;
	}
  }
  strcat(ret, itoa(num, str_num, 10)); 
  return ret;
}

int main() {
  char* str = "aaabbadddffc";
  char* tmp = statics_chara(str, strlen(str));
  printf("ret=%s\n", tmp);
  return 0;
}
