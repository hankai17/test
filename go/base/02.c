#include<stdio.h>

int main() {
  int* arr[4] = {};
  
  {
    int tmp = 99;
    arr[0] = &tmp;
  }
  printf("%p == %d\n", arr[0], (int)*(arr[0]));

  return 0;
}
