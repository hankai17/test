#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int main1(int argc, char** argv) {
  if(argc != 2) return 0;
  srand(time(NULL));
  for(int i = 0; i < 3; i++) {
    int ret = rand() % (int)atoi(argv[1]);
    printf("%d\n", ret + 1);
  }
  return 0;
}

float get_power(float m, int power) { //m^power
  if(power == 0) return 1;
  float base = m;
  for(int i = 1; i < power; i++) {
    m = m * base;
  }
  return m;
}

int main2() {
  for(int i = 0; i < 4; i++) {
	printf("%d  %f\n", i, get_power(0.5, i));
  }
  return 0;
}

int main(int argc, char** argv) {
  float cur;
  float sum = 0;
  for(int i = 1; i < 10; i++) {
	if(i % 2 != 0) {
	  cur = get_power(0.5, i);
      printf("%d  %f\n",i, cur);
	  sum += cur;
    }
  }
  printf("%f\n", sum);
  return 0;
}
