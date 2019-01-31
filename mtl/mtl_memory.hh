#ifndef _mtl_memory_hh_
#define	_mtl_memory_hh_

#include <ctype.h>
#include <strings.h>
#include <stdlib.h>
#include <malloc.h>

#ifdef KERNEL
  typedef void * caddr_t;
#endif

//#include "mtl_config.h"

#if HAVE_UNISTD_H
#include <unistd.h>
#endif

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_SYS_MMAN_H
#include <sys/mman.h>
#endif

#if TS_HAS_JEMALLOC
#include <jemalloc/jemalloc.h>
/* TODO: Should this have a value ? */
#define MTL_MMAP_MAX 0
#else
#if HAVE_MALLOC_H
#include <malloc.h>
#define MTL_MMAP_MAX M_MMAP_MAX
#endif // ! HAVE_MALLOC_H
#endif // ! TS_HAS_JEMALLOC


#ifndef MADV_NORMAL
#define MADV_NORMAL 0
#endif

#ifndef MADV_RANDOM
#define MADV_RANDOM 1
#endif

#ifndef MADV_SEQUENTIAL
#define MADV_SEQUENTIAL 2
#endif

#ifndef MADV_WILLNEED
#define MADV_WILLNEED 3
#endif

#ifndef MADV_DONTNEED
#define MADV_DONTNEED 4
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

  void *  mtl_malloc(size_t size);
  void *  mtl_calloc(size_t nelem, size_t elsize);
  void *  mtl_realloc(void *ptr, size_t size);
  void *  mtl_memalign(size_t alignment, size_t size);
  void    mtl_free(void *ptr);
  void *  mtl_free_null(void *ptr);
  void    mtl_memalign_free(void *ptr);
  int     mtl_mallopt(int param, int value);

  int     mtl_msync(void *addr, size_t len, void *end, int flags);
  int     mtl_madvise(void *addr, size_t len, int flags);
  int     mtl_mlock(void *addr, size_t len);

  static inline size_t __attribute__((const)) mtl_pagesize(void)
  {
    static size_t page_size;

    if (page_size)
      return page_size;

#if defined(HAVE_SYSCONF) && defined(_SC_PAGESIZE)
    page_size = (size_t)sysconf(_SC_PAGESIZE);
#elif defined(HAVE_GETPAGESIZE)
    page_size = (size_t)getpagesize()
#else
    page_size = (size_t)8192;
#endif

    return page_size;
  }

#define mtl_strdup(p)        _xstrdup((p), -1, NULL)
#define mtl_strndup(p,n)     _xstrdup((p), n, NULL)

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/** Set data to zero.

    Calls @c memset on @a t with a value of zero and a length of @c
    sizeof(t). This can be used on ordinary and array variables. While
    this can be used on variables of intrinsic type it's inefficient.

    @note Because this uses templates it cannot be used on unnamed or
    locally scoped structures / classes. This is an inherent
    limitation of templates.

    Examples:
    @code
    foo bar; // value.
    mtl_zero(bar); // zero bar.

    foo *bar; // pointer.
    mtl_zero(bar); // WRONG - makes the pointer @a bar zero.
    mtl_zero(*bar); // zero what bar points at.

    foo bar[ZOMG]; // Array of structs.
    mtl_zero(bar); // Zero all structs in array.

    foo *bar[ZOMG]; // array of pointers.
    mtl_zero(bar); // zero all pointers in the array.
    @endcode
    
 */
template < typename T > inline void
mtl_zero(
	 T& t ///< Object to zero.
	 ) {
  memset(&t, 0, sizeof(t));
}
#endif  /* __cplusplus */

#endif
