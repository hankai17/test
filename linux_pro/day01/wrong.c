#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
  if(argc < 1) return 0;
  printf("arg:%s\n", argv[0]);
  return 0;
}

