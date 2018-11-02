#include<stdio.h>
#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<algorithm>
using namespace std;

//

int min_interval(char* str, int len, char m, char n) {
  if(str == NULL || len == 0) return -1;
  if(m == n) return 0;
  int last1 = -1;
  int last2 = -1;
  int min = 0x0FFFFFFF;
  for(int i = 0; i < len; i++) {
	if(str[i] == m) {
	  min = std::min(min, last2 == -1 ? min : i - last2);
	  last1 = i;
    }
	if(str[i] == n) {
	  min = std::min(min, last1 == -1 ? min : i - last1);
	  last2 = i;
    }
  }
  return min == 0x0FFFFFFF ? -1 : min;
}
int main() {
  char str[] = "mike love dog";
  int ret = min_interval(str, strlen(str), 'e', 'o'); 
  printf("%d\n", ret);
  return 0;
}
