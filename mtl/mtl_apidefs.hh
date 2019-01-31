#ifndef _mtl_apidefs_hh_
#define _mtl_apidefs_hh_

#define mtlliapi
#define mtlcoreapi
#define mtl_undoc_liapi
#define mtl_undoc_coreapi mtlcoreapi

#if defined(__GNUC__) || defined(__clang__)
#ifndef likely
#define likely(x)	__builtin_expect (!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)	__builtin_expect (!!(x), 0)
#endif
#else
#ifndef likely
#define likely(x)	(x)
#endif
#ifndef unlikely
#define unlikely(x)	(x)
#endif
#endif

#if !defined(MTL_NORETURN)
#if defined(__GNUC__) || defined(__clang__)
#define MTL_NORETURN __attribute__((noreturn))
#else
#define MTL_NORETURN
#endif
#endif

/*  Enable this to get printf() style warnings on the Inktomi functions. */
/* #define PRINTFLIKE(IDX, FIRST)  __attribute__((format (printf, IDX, FIRST))) */
#if !defined(MTL_PRINTFLIKE)
#if defined(__GNUC__) || defined(__clang__)
#define MTL_PRINTFLIKE(fmt, arg) __attribute__((format(printf, fmt, arg)))
#else
#define MTL_PRINTFLIKE(fmt, arg)
#endif
#endif

#if !defined(MTL_INLINE)
#define MTL_INLINE inline
#endif

#endif
