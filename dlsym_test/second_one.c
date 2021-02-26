#include <stdio.h>
void print_message()
{
      printf("the second lib~~\n");
}
 
void second()
{
      printf("init second \n");
}
// gcc -fpic --shared second_one.c -o libsecond_one.so
