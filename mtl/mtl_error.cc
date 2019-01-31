//#include "libts.h"
#include "mtl_error.hh"
#include "mtl_stack_trace.hh"
#include <stdlib.h>
#include <unistd.h>

#include <syslog.h>
#include <signal.h>    /* MAGIC_EDITING_TAG */

static int mtl_dprintf_level = 0;

/**
  This routine causes process death. Some signal handler problems got
  in the way of abort before, so this is an overzealous and somewhat
  amusing implementation.

*/
void
mtl_die_die_die(int retval)
{
  abort();
  _exit(retval);
  exit(retval);
}

/**
  This routine prints/logs an error message given the printf format
  string in message_format, and the optional arguments.  The program is
  then terminated with return_code.

*/
void
mtl_fatal_va(int return_code, const char *message_format, va_list ap)
{
  char extended_format[4096], message[4096];
  snprintf(extended_format, sizeof(extended_format) - 1, "FATAL: %s", message_format);
  extended_format[sizeof(extended_format) - 1] = 0;
  vsnprintf(message, sizeof(message) - 1, extended_format, ap);
  message[sizeof(message) - 1] = 0;
  fprintf(stderr, "%s\n", message);
  syslog(LOG_CRIT, "%s", message);
  mtl_stack_trace_dump();
  mtl_die_die_die(return_code);
}

void
mtl_fatal(int return_code, const char *message_format, ...)
{
  va_list ap;
  va_start(ap, message_format);
  mtl_fatal_va(return_code, message_format, ap);
  va_end(ap);
}

void
mtl_fatal_die(const char *message_format, ...)
{
  va_list ap;
  va_start(ap, message_format);
  mtl_fatal_va(1, message_format, ap);
  va_end(ap);
}

/**
  This routine prints/logs an error message given the printf format
  string in message_format, and the optional arguments.  The current
  errno is also printed.  The program is then terminated with return_code.

*/
void
mtl_pfatal(int return_code, const char *message_format, ...)
{
  va_list ap;
  char extended_format[4096], message[4096];

  char *errno_string;

  va_start(ap, message_format);
  errno_string = strerror(errno);
  snprintf(extended_format, sizeof(extended_format) - 1, "FATAL: %s <last errno = %d (%s)>",
           message_format, errno, (errno_string == NULL ? "unknown" : errno_string));
  extended_format[sizeof(extended_format) - 1] = 0;
  vsnprintf(message, sizeof(message) - 1, extended_format, ap);
  message[sizeof(message) - 1] = 0;
  fprintf(stderr, "%s\n", message);
  syslog(LOG_CRIT, "%s", message);
  va_end(ap);
  mtl_stack_trace_dump();
  mtl_die_die_die(return_code);
}

/**
  This routine prints/logs a warning message given the printf format
  string in message_format, and the optional arguments.

*/
void
mtl_warning(const char *message_format, ...)
{
  va_list ap;
  char extended_format[4096], message[4096];
  va_start(ap, message_format);
  snprintf(extended_format, sizeof(extended_format) - 1, "WARNING: %s", message_format);
  extended_format[sizeof(extended_format) - 1] = 0;
  vsnprintf(message, sizeof(message) - 1, extended_format, ap);
  message[sizeof(message) - 1] = 0;
  fprintf(stderr, "%s\n", message);
  syslog(LOG_WARNING, "%s", message);
  va_end(ap);
}

/**
  This routine prints/logs a warning message given the printf format
  string in message_format, and the optional arguments.  The current
  errno is also printed.

*/
void
mtl_pwarning(const char *message_format, ...)
{
  va_list ap;
  char extended_format[4096], message[4096];
  char *errno_string;

  va_start(ap, message_format);
  errno_string = strerror(errno);
  snprintf(extended_format, sizeof(extended_format) - 1, "WARNING: %s <last errno = %d (%s)>",
           message_format, errno, (errno_string == NULL ? "unknown" : errno_string));
  extended_format[sizeof(extended_format) - 1] = 0;
  vsnprintf(message, sizeof(message) - 1, extended_format, ap);
  message[sizeof(message) - 1] = 0;
  fprintf(stderr, "%s\n", message);
  syslog(LOG_WARNING, "%s", message);
  va_end(ap);
}

/**
  This routine prints/logs a notice message given the printf format
  string in message_format, and the optional arguments.

*/
void
mtl_notice(const char *message_format, ...)
{
  va_list ap;
  char extended_format[4096], message[4096];
  va_start(ap, message_format);
  snprintf(extended_format, sizeof(extended_format) - 1, "NOTE: %s", message_format);
  extended_format[sizeof(extended_format) - 1] = 0;
  vsnprintf(message, sizeof(message) - 1, extended_format, ap);
  message[sizeof(message) - 1] = 0;
  fprintf(stderr, "%s\n", message);
  syslog(LOG_NOTICE, "%s", message);
  va_end(ap);
}

/**
  This routine prints/logs a message given the printf format string in
  message_format, and the optional arguments.

*/
void
mtl_eprintf(const char *message_format, ...)
{
  va_list ap;
  char message[4096];
  va_start(ap, message_format);
  vsnprintf(message, sizeof(message) - 1, message_format, ap);
  message[sizeof(message) - 1] = 0;
  fprintf(stderr, "ERROR: %s\n", message);
  va_end(ap);
}

/**
  This routine prints/logs a warning message given the printf format
  string in message_format, and the optional arguments.

*/
void
mtl_error(const char *message_format, ...)
{
  va_list ap;
  char extended_format[2048], message[4096];
  va_start(ap, message_format);
  snprintf(extended_format, sizeof(extended_format) - 1, "ERROR: %s", message_format);
  extended_format[sizeof(extended_format) - 1] = 0;
  vsnprintf(message, sizeof(message) - 1, extended_format, ap);
  message[sizeof(message) - 1] = 0;
  fprintf(stderr, "%s\n", message);
  syslog(LOG_ERR, "%s", message);
  va_end(ap);
}

/**
  This routine prints/logs a message given the printf format string in
  message_format, and the optional arguments.

*/
void
mtl_dprintf(int debug_level, const char *message_format, ...)
{
  char message[4096];
  va_list ap;
  if (debug_level <= mtl_dprintf_level) {
    va_start(ap, message_format);
    vsnprintf(message, sizeof(message) - 1, message_format, ap);
    message[sizeof(message) - 1] = 0;
    fprintf(stderr, "%s\n", message);
    va_end(ap);
  }
}

/**
  Set output level for mtl_dprintf() function. For debugging purposes
  only!

*/
int
mtl_set_dprintf_level(int debug_level)
{
  int old_mtl_dprintf_level = mtl_dprintf_level;
  if ((mtl_dprintf_level = debug_level) < 0)
    mtl_dprintf_level = 0;
  return old_mtl_dprintf_level;
}

/**
  Initiates a SEGV the old-fashioned way, it earns it.

*/
void
mtl_segv()
{
  char *addr = NULL;
  *addr = 0;
  mtl_die_die_die(1); // just keep clang happy ... it thmtls we return from here ...
}
