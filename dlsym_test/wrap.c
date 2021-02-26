# define RTLD_NEXT      ((void *) -1l)
#include <stdio.h>
#include <dlfcn.h>
#include <errno.h>
void(*f)();
void load_func() __attribute__((constructor));
void load_func()
{
      f = (void(*)())dlsym(RTLD_NEXT,"print_message");
          char *error_str;
              error_str = dlerror();
                  if (error_str != NULL) {
                            printf("%s\n", error_str);
                                }
                      printf("load func first f=%p\n",f);
                       
}
void print_message_wrap()
{
      printf("the wrap lib~~\n");
          f();
}

// gcc -fpic --shared wrap.c -o libwrap.so
// void load_func() __attribute__((constructor))的含义是在执行main函数前，执行load_func这个函数，便于我们做一些准备工作。显然这里的作用就是触发dlsym以实现查找第一个"print_message"函数符号的目的。
