#include<stdio.h>
#include<string.h>
typedef int bool;
#define true 1
#define false 0

//str="a1.2e33" 结果为36
//str="a-1bc--12" 为11
//"a-1b--2c--d6e" 为7
//首先确定有三个变量sum num pos

int get_sum(char* str, int len) {
  if(str == NULL || len == 0) {
	return 0;
  }
  int sum = 0;
  int num = 0;
  bool pos = true;
  for(int i = 0; i < len; i++) {
	int cur = str[i] - '0';
	if(cur < 0 || cur > 9) {
	  sum += num;
	  num = 0;	
      if(str[i] == '-') {
		//if(str[i - 1] == '-') pos = !pos; //妙
		if(i > 0 && str[i - 1] == '-') pos = !pos; //妙
		else pos = false;
      } else {
		pos = true;
	  }
    } else {
	  num = num * 10 + (pos ? cur : -cur);
    }
  }
  sum += num; //忘了写
  return sum;
}

int main() {
  char* str = "a-1b-2c--d6e";
  int ret = get_sum(str, strlen(str));
  printf("ret = %d\n", ret);
  return 0;
}
