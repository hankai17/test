//Error reporting routines for libts
#ifndef _mtl_error_hh_
#define	_mtl_error_hh_

#include <stdarg.h>
#include "mtl_platform.hh"
#include "mtl_apidefs.hh"

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

void mtl_fatal_va(int return_code, const char *message_format, va_list ap) MTL_NORETURN;
void mtl_fatal(int return_code, const char *message_format, ...) MTL_PRINTFLIKE(2, 3) MTL_NORETURN;
void mtl_pfatal(int return_code, const char *message_format, ...) MTL_PRINTFLIKE(2, 3) MTL_NORETURN;
void mtl_warning(const char *message_format, ...) MTL_PRINTFLIKE(1, 2);
void mtl_pwarning(const char *message_format, ...) MTL_PRINTFLIKE(1, 2);
void mtl_notice(const char *message_format, ...) MTL_PRINTFLIKE(1, 2);
void mtl_eprintf(const char *message_format, ...) MTL_PRINTFLIKE(1, 2);
void mtl_error(const char *message_format, ...) MTL_PRINTFLIKE(1, 2);
void mtl_dprintf(int debug_level, const char *message_format, ...) MTL_PRINTFLIKE(2, 3);
void mtl_fatal_die(const char *message_format, ...) MTL_PRINTFLIKE(1, 2) MTL_NORETURN;

void mtl_die_die_die(int retval) MTL_NORETURN;
void mtl_segv() MTL_NORETURN;
int mtl_set_dprintf_level(int debug_level);

#ifdef __cplusplus
}
#endif                          /* __cplusplus */

#endif
