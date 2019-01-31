#ifndef _MTL_ASSERT_HH
#define _MTL_ASSERT_HH

//#include "mtl_apidefs.h"
#include "mtl_error.hh"

#ifdef __cplusplus
extern "C"
{
#endif                          /* __cplusplus */

/* don't use assert, no really DON'T use assert */
#undef assert
#define assert __DONT_USE_BARE_assert_USE_mtl_assert__
#undef _ASSERT_H
#define _ASSERT_H
#undef __ASSERT_H__
#define __ASSERT_H__

  void _mtl_assert(const char *a, const char *f, int l) MTL_NORETURN;

#if defined(DEBUG) || defined(__clang_analyzer__)
#define mtl_assert(EX) ( \
            (void)(likely(EX) ? (void)0 : _mtl_assert(#EX, __FILE__, __LINE__))\
)
#else
#define mtl_assert(EX) (void)(EX)
#endif

#define mtl_release_assert(EX) ( \
            (void)(likely(EX) ? (void)0 : _mtl_assert(#EX, __FILE__, __LINE__)) \
)

#ifdef __cplusplus
}
#endif

#endif

/* workaround a bug in the  stupid Sun preprocessor

#undef assert
#define assert __DONT_USE_BARE_assert_USE_mtl_assert__
#define _ASSERT_H
#undef __ASSERT_H__
#define __ASSERT_H__
*/
