#include <stdio.h>
#include <unistd.h>

int main(void) {
  int i;
  alarm(1);
  for(i = 0; ; i++)
  printf("%d\n", i);
  return 0;
}

//time a.out
//time a.out > 1.txt
//io优化才是核心
