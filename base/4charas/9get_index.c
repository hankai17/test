#include<stdio.h>
#include<string.h>
#include<stdlib.h>
typedef int bool;
#define true 1
#define false 0

int get_index(char* str, int len, char k) {
  if(str == NULL || len == 0) return -1;
  int ret = -1;
  int l = 0;
  int r = len - 1;
  int m = 0;
  int i = 0;
  while( l < r) {
	m = (l + r) / 2;
	if(str[m] != 'a' && str[m] == k) { //中间值恰好等于 继续向左查找
	  ret = m;
 	  r = m - 1; //向左找最小的那个
	} else if(str[m] != 'a') { //中间值不等于 重新定位
	  if(str[m] < k) l = m + 1;	
	  else r = m - 1;
	} else { //中间值为null
	  i = m;
	  while(str[i] == 'a' && --i >= l); //向左移动到不null的地方
	  if(i < l || str[i] < k) {
		l = m + 1;
	  } else {
		ret = str[i] == k ? i : ret;
	    r = i - 1;
	  }
	}
  }
  return ret;
}

int main() {
  char str[] = "10a23a57a9";
  int ret = get_index(str, strlen(str), '3');
  printf("ret=%d\n", ret);
  return 0;
}

