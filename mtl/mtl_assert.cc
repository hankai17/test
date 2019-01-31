#include "mtl_platform.hh"
#include "mtl_assert.hh"
#include "mtl_error.hh"
//#include "mtl_string.h"

void
_mtl_assert(const char *expression, const char *file, int line)
{
  mtl_fatal(1, "%s:%d: failed assert `%s`", file, line, expression);
}
