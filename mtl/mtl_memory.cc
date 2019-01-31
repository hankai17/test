//#include "libts.h"
#include "mtl_apidefs.hh"
#include "mtl_stack_trace.hh"
#include "mtl_error.hh"
#include "mtl_memory.hh"
#include <stdint.h>

#include <assert.h>
#if defined(linux)
// XXX: SHouldn't that be part of CPPFLAGS?
#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 600
#endif
#endif
#include <stdlib.h>
#include <string.h>

void *
mtl_malloc(size_t size)
{
  void *ptr = NULL;

  /*
   * There's some nasty code in libts that expects
   * a MALLOC of a zero-sized item to work properly. Rather
   * than allocate any space, we simply return a NULL to make
   * certain they die quickly & don't trash things.
   */

  // Useful for tracing bad mallocs
  // mtl_stack_trace_dump();
  if (likely(size > 0)) {
    if (unlikely((ptr = malloc(size)) == NULL)) {
      mtl_stack_trace_dump();
      mtl_fatal(1, "mtl_malloc: couldn't allocate %zu bytes", size);
    }
  }
  return ptr;
}

void *
mtl_calloc(size_t nelem, size_t elsize)
{
  void *ptr = calloc(nelem, elsize);
  if (unlikely(ptr == NULL)) {
    mtl_stack_trace_dump();
    mtl_fatal(1, "mtl_calloc: couldn't allocate %zu %zu byte elements", nelem, elsize);
  }
  return ptr;
}

void *
mtl_realloc(void *ptr, size_t size)
{
  void *newptr = realloc(ptr, size);
  if (unlikely(newptr == NULL)) {
    mtl_stack_trace_dump();
    mtl_fatal(1, "mtl_realloc: couldn't reallocate %zu bytes", size);
  }
  return newptr;
}

// TODO: For Win32 platforms, we need to figure out what to do with memalign.
// The older code had ifdef's around such calls, turning them into mtl_malloc().
void *
mtl_memalign(size_t alignment, size_t size)
{
  void *ptr;

#if HAVE_POSIX_MEMALIGN || MTL_HAS_JEMALLOC
  if (alignment <= 8)
    return mtl_malloc(size);

#if defined(openbsd)
  if (alignment > PAGE_SIZE)
      alignment = PAGE_SIZE;
#endif
  mtl_assert((alignment & (alignment - 1)) == 0); //ensure alignment = 2^x
  int retcode = posix_memalign(&ptr, alignment, size); 

  if (unlikely(retcode)) {
    if (retcode == EINVAL) {
      mtl_fatal(1, "mtl_memalign: couldn't allocate %zu bytes at alignment %zu - invalid alignment parameter",
                size, alignment);
    } else if (retcode == ENOMEM) {
      mtl_fatal(1, "mtl_memalign: couldn't allocate %zu bytes at alignment %zu - insufficient memory",
                size, alignment);
    } else {
      mtl_fatal(1, "mtl_memalign: couldn't allocate %zu bytes at alignment %zu - unknown error %d",
                size, alignment, retcode);
    }
  }
#else
  ptr = memalign(alignment, size);
  if (unlikely(ptr == NULL)) {
    mtl_fatal(1, "mtl_memalign: couldn't allocate %zu bytes at alignment %zu",  size,  alignment);
  }
#endif
  return ptr;
}

void
mtl_free(void *ptr) //TODO WHY not end ptr null
{
  if (likely(ptr != NULL))
    free(ptr);
}

void*
mtl_free_null(void *ptr)
{
  if (likely(ptr != NULL))
    free(ptr);
  return NULL;
}

void
mtl_memalign_free(void *ptr)
{
  if (likely(ptr))
    free(ptr);
}

// This effectively makes mallopt() a no-op (currently) when tcmalloc
// or jemalloc is used. This might break our usage for increasing the
// number of mmap areas (ToDo: Do we still really need that??).
//
// TODO: I thmtl we might be able to get rid of this?
int
mtl_mallopt(int param, int value)
{
#if MTL_HAS_JEMALLOC
// TODO: jemalloc code ?
#else
#if MTL_HAS_TCMALLOC
// TODO: tcmalloc code ?
#else
#if defined(linux)
  return mallopt(param, value); //https://www.cnblogs.com/dongzhiquan/p/5621906.html 关于malloc mmap分配原理 以及碎片信息
#endif
#endif
#endif
  return 0;
}

int
mtl_msync(void *addr, size_t len, void *end, int flags) //mmap函数专用
{
  size_t pagesize = mtl_pagesize();
  // align start back to page boundary
  void *a =  (void*)(((uintptr_t) addr) & ~(pagesize - 1));
  // align length to page boundry covering region
  size_t l = (len + ((size_t)addr - (size_t)a) + (pagesize - 1)) & ~(pagesize - 1);

  if (((size_t)a + (size_t)l) - (size_t)end > 0)
    l = (size_t)end - (size_t)a;   // strict limit
#if defined(linux)
/* Fix INKqa06500
   Under Linux, msync(..., MS_SYNC) calls are painfully slow, even on
   non-dirty buffers. This is true as of kernel 2.2.12. We sacrifice
   restartability under OS in order to avoid a nasty performance hit
   from a kernel global lock. */
#if 0
  // this was long long ago
  if (flags & MS_SYNC)
    flags = (flags & ~MS_SYNC) | MS_ASYNC;
#endif
#endif
  int res = msync(a, l, flags);
  return res;
}

int
mtl_madvise(void *addr, size_t len, int flags) //mmap专用
{
#if defined(linux)
  (void) addr;
  (void) len;
  (void) flags;
  return 0;
#else
  size_t pagesize = mtl_pagesize();
  void *a = (void*) (((uintptr_t) addr) & ~(pagesize - 1));
  size_t l = (len + (addr - a) + pagesize - 1) & ~(pagesize - 1);
  int res = 0;
#if HAVE_POSIX_MADVISE
  res = posix_madvise(a, l, flags);
#else
  res = madvise(a, l, flags);
#endif
  return res;
#endif
}

int
mtl_mlock(void *addr, size_t len)
{
  size_t pagesize = mtl_pagesize();

  void *a = (void*) (((uintptr_t) addr) & ~(pagesize - 1)); //a & p-1 a对p取余   前提是p必须是2^x
  size_t l = (len + ((size_t)addr - (size_t)a) + pagesize - 1) & ~(pagesize - 1); //x-1+p 对p取余
  int res = mlock(a, l);
  return res;
}
