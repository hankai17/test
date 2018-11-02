#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <event2/event.h>

int main(int argc, char *argv[]) {
  struct event_base *base = NULL;
  base = event_base_new();

  int i;
  const char** methods = event_get_supported_methods();
  printf("Starting Libevent %s. Available methods are:\n", event_get_version());
  for (i=0; methods[i] != NULL; ++i) {
	printf(" %s\n", methods[i]);
  }

  //2 向base中添加一些事件。
  //3 阻塞监听事件状态
  event_base_dispatch(base);

  //4 释放base
  event_base_free(base);
  return 0;
}
