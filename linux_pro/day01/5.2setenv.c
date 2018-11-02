#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char *val;
  const char *name = "ABD";

  val = getenv(name);
  printf("%s = %s\n", name, val);
  setenv(name, "haha-day-and-night", 1);
  val = getenv(name);
  printf("%s = %s\n", name, val);

#if 0
  unsetenv("ABD");
  val = getenv(name);
  printf("%s = %s\n", name, val);
#else
  int ret = unsetenv("ABCEFJEW=");  //name=value:value
  if(ret = -1) {
    printf("ret = %d\n", ret);
	perror("err");
  }

  val = getenv(name);
  printf("%s = %s\n", name, val);
#endif

  return 0;
}

int main1(void) {
  printf("PATH=%s\n", getenv("PATH"));
  //setenv("PATH", "hello", 1);
  //printf("PATH=%s\n", getenv("PATH"));
  return 0;
}
