#include <stdio.h>
void print_message()
{
      printf("the first lib~~\n");
}
void first()
{
      printf("init first\n");
}
// gcc -fpic --shared first_one.c -o libfirst_one.so
