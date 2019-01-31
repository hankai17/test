#ifndef _mtl_defs_hh
#define	_mtl_defs_hh

//#include "ink_config.h"
#include <stddef.h>

#ifdef HAVE_STDINT_H
# ifndef __STDC_LIMIT_MACROS
#  define __STDC_LIMIT_MACROS 1
# endif
# include <stdint.h>
#else
// TODO: Add "standard" int types?
#endif

#ifdef HAVE_INTTYPES_H
# ifndef __STDC_FORMAT_MACROS
#  define __STDC_FORMAT_MACROS 1
# endif
# include <inttypes.h>
#else
// TODO: add PRI*64 stuff?
#endif

#ifndef INT64_MIN
#define INT64_MAX (9223372036854775807LL) //2^63 - 1
#define INT64_MIN (-INT64_MAX -1LL)
#define INTU32_MAX (4294967295U)
#define INT32_MAX (2147483647)
#define INT32_MIN (-2147483647-1)
#endif
// Hack for MacOSX, have to take this out of the group above.
#ifndef INTU64_MAX
#define INTU64_MAX (18446744073709551615ULL)
#endif
/*
#define POSIX_THREAD
#define POSIX_THREAD_10031c

#ifndef ETIME
#ifdef ETIMEDOUT
#define ETIME ETIMEDOUT
#endif
#endif

#ifndef ENOTSUP
#ifdef EOPNOTSUPP
#define ENOTSUP EOPNOTSUPP
#endif
#endif

#if defined(darwin)
#define RENTRENT_GETHOSTBYNAME
#define RENTRENT_GETHOSTBYADDR
#endif
*/

#define NUL '\0'

// Need to use this to avoid problems when calling variadic functions with many arguments. In such cases, a raw '0' or NULL can be interpreted as 32 bits
#define NULL_PTR static_cast<void*>(0) //没有运行时类型检查来保证转换的安全性

/*
// Determine the element count for an array.
#ifdef __cplusplus
template<typename T, unsigned N>
static inline unsigned
countof(const T (&)[N]) {
  return N;
}
#else
#  define countof(x) ((unsigned)(sizeof(x)/sizeof((x)[0])))
#endif
*/
/*
#define SOCKOPT_ON ((char*)&on)
#define SOCKOPT_OFF ((char*)&off)
*/

#ifndef ABS
#define ABS(x) (((x) < 0) ? ( - (x)) : (x))
#endif

#ifndef MAX
#define MAX(x,y) (((x) >= (y)) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x,y) (((x) <= (y)) ? (x) : (y))
#endif

#define MTL_UNUSED __attribute__ ((unused))
#define MTL_WARN_IF_UNUSED __attribute__ ((warn_unused_result))
#define	MTL_UNUSED_RETURN(x)	if (x) {}

#ifndef likely
#define likely(x)	__builtin_expect (!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)	__builtin_expect (!!(x), 0)
#endif

/*
#if TS_USE_HWLOC
#  include <hwloc.h>
#endif
*/

/*
#ifndef ROUNDUP
#define ROUNDUP(x, y) ((((x)+((y)-1))/(y))*(y))
#endif
*/

/* Debugging
*/
/*
#ifdef NDEBUG

#define FDBG
#define DBG(s)
#define DBG1(s,a)
#define DBG2(s,a1,a2)
#define DBG3(s,a1,a2,a3)
#define DBG4(s,a1,a2,a3,a4)

#else
*/
#define FDBG                if (debug_level==1) printf("debug "__FILE__":%d %s : entered\n" ,__LINE__,__FUNCTION__)
#define DBG(s)              if (debug_level==1) printf("debug "__FILE__":%d %s :" s ,__LINE__,__FUNCTION__)
#define DBG1(s,a)           if (debug_level==1) printf("debug "__FILE__":%d %s :" s ,__LINE__,__FUNCTION__, a)
#define DBG2(s,a1,a2)       if (debug_level==1) printf("debug "__FILE__":%d %s :" s ,__LINE__,__FUNCTION__, a1,a2)
#define DBG3(s,a1,a2,a3)    if (debug_level==1) printf("debug "__FILE__":%d %s :" s ,__LINE__,__FUNCTION__, a1,a2,a3)
#define DBG4(s,a1,a2,a3,a4) if (debug_level==1) printf("debug "__FILE__":%d %s :" s ,__LINE__,__FUNCTION__, a1,a2,a3,a4)

//#endif

/* Types
*/
/*
typedef void *(*VPVP_PFN) (void *);
typedef void (*VVP_PFN) (void *);
typedef void (*VV_PFN) (void);
typedef void (*VI_PFN) (int);
*/

/* Variables
*/
extern int debug_level;
extern int off;
extern int on;

/* Functions
*/
int mtl_sys_name_release(char *name, int namelen, char *release, int releaselen);
int mtl_number_of_processors();
/*
#if TS_USE_HWLOC
// Get the hardware topology
hwloc_topology_t ink_get_topology();
#endif
*/

/** Constants.
 */
#ifdef __cplusplus
namespace ts {
  static const int NO_FD = -1; ///< No or invalid file descriptor.
}
#endif

#endif
