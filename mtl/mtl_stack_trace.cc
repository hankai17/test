//#include "libts.h"
#include "mtl_stack_trace.hh"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if MTL_HAS_BACKTRACE

#include <execinfo.h>           /* for backtrace_symbols, etc. */
#include <signal.h>

void
mtl_stack_trace_dump()
{
  int btl;

  // Recopy and re-terminate the app name in case it has been trashed.
  char name[256];
  const char *msg = " - STACK TRACE: \n";
  mtl_strlcpy(name, program_name, sizeof(name));
  if (write(2, name, strlen(name)) == -1)
    return;
  if (write(2, msg, strlen(msg)) == -1)
    return;

  void *stack[INK_STACK_TRACE_MAX_LEVELS + 1];
  memset(stack, 0, sizeof(stack));
  if ((btl = backtrace(stack, INK_STACK_TRACE_MAX_LEVELS)) > 2) {
    // dump the backtrace to stderr
    backtrace_symbols_fd(stack + 2, btl - 2, 2);
  }
}

#else

void
mtl_stack_trace_dump()
{
  const char msg[] = "mtl_stack_trace_dump not implemented on this operating system\n";
  if (write(2, msg, sizeof(msg) - 1) == -1)
      return;
}

#endif
