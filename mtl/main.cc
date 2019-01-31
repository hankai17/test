#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "mtl_defs.hh"
#include "mtl_code.hh"
#include "mtl_md5.hh"
#include "mtl_rand.hh"
#include "mtl_memory.hh"

int test_release() {
  char name[64] = {0};
  char release[64] = {0};
  mtl_sys_name_release(name, sizeof(name) - 1, release, sizeof(release) - 1);
  printf("name: %s, release: %s\n", name, release);

  int ret = mtl_number_of_processors();
  printf("processors: %d\n", ret);
  return 0;
}

void test_md5() {
  unsigned char para[64] = "hello";
  unsigned char md5[17] = {0};
  //mtl_code_md5(para, sizeof(para), md5);
  mtl_code_md5(para, strlen((char*)para), md5); //not sizeof
  printf("md5: %s len: %d\n", md5, strlen((char*)md5));

  char string_md5[33] = {0};
  mtl_code_md5_stringify(string_md5, sizeof(string_md5) - 1, (char*)md5);
  printf("string md5: %s\n", string_md5);

  char string_md51[33] = {0};
  mtl_code_md5_stringify_fast(string_md51, (char*)md5);
  printf("string md5: %s\n", string_md51);
  return;
}

void test_md5_c() {
  MTL_MD5 cachekey; 
/*
  unsigned char para[64] = "hello";
  cachekey.encodeBuffer(para, strlen(para));
*/
  char para[64] = "hello";
  cachekey.encodeBuffer(para, strlen(para));
  
  char out[33] = {0};
  printf("string: %s\n", cachekey.string(out));
  printf("toHexStr: %s\n", cachekey.toHexStr(out));
  //printf("toStr: %s\n", cachekey.toStr());
  printf("str: %s\n", cachekey.str());
  return;
}

void test_rand() {
  MtlRand r(17);
  printf("r: %d\n", r.random() % 9);
  printf("r: %d\n", r.random() % 9);
  printf("r: %d\n", r.random() % 9);
  return;
}

int test_malloc_memory(char *p , int size)
{
    p = (char *)malloc(size);
    if(!p) {
        printf("malloc error  \n");
        return -1;
    }

    printf("len of malloc(%d)  is  %d  ,the real is %d\n",size, strlen(p), malloc_usable_size(p)); //realsize为8的倍数
    printf("first member of malloc(%d) is %p:%d \n",size, p, *p);

    *p = 10;
    printf("after set the first member of malloc(%d) is %p:%d \n",size, p, *p);

    memset(p,'\0',12);
    memcpy(p, "01234567890123456789", 12);
    printf("after memcpy , the content is %s   len is %d  , the ture is %d \n",p,strlen(p),malloc_usable_size(p));

    free(p);
    p = NULL;
    printf("\n");
}


void test_malloc() {
    char *p = NULL;
    int size = 0;
    test_malloc_memory(p,size);

    size = 5;
    test_malloc_memory(p,size);

    size = 25;
    test_malloc_memory(p,size);

    return;
}

void test_mtl_malloc() {
    char *p = NULL; 
    p = (char*) mtl_malloc(1024);
    mtl_free_null(p); 

    p = (char*) mtl_calloc(1, 1024);
    //mtl_free_null(p); 

    p = (char*) mtl_realloc(p, 2048);
    mtl_free_null(p); 

    p = (char*) mtl_memalign(512, 1024);
    p = (char*) mtl_memalign(521, 1024);
    mtl_free_null(p); 
}

int main() {
  //test_md5_c();
  //test_rand();
  //test_malloc();
  test_mtl_malloc();
  return 0;
}


