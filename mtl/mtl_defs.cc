//#include "libts.h"
#include "mtl_defs.hh"
#include "mtl_platform.hh"
#if defined(linux) || defined(freebsd) || defined(darwin)
#include <sys/types.h>
#include <sys/param.h>
#include <sys/sysctl.h>
#include <unistd.h>
#endif
#if defined(linux)
#include <sys/utsname.h>
#endif      /* MAGIC_EDITING_TAG */

int off = 0;
int on = 1;
/*
#if TS_USE_HWLOC

#include <hwloc.h>
*/

// Little helper to initialize the hwloc topology, once.
/*
static hwloc_topology_t
setup_hwloc()
{
  hwloc_topology_t topology;

  hwloc_topology_init(&topology);
  hwloc_topology_load(topology);

  return topology;
}

// Get the topology
hwloc_topology_t
ink_get_topology()
{
  static hwloc_topology_t topology = setup_hwloc();
  return topology;
}

#endif
*/

int
mtl_sys_name_release(char *name, int namelen, char *release, int releaselen)
{
  *name = 0;
  *release = 0;
#if defined(freebsd) || defined(darwin)
  int mib[2];
  size_t len = namelen;
  mib[0] = CTL_KERN;
  mib[1] = KERN_OSTYPE;

  if (sysctl(mib, 2, name, &len, NULL, 0) == -1)
    return -1;

  len = releaselen;
  mib[0] = CTL_KERN;
  mib[1] = KERN_OSRELEASE;

  if (sysctl(mib, 2, release, &len, NULL, 0) == -1)
    return -1;

  return 0;
#elif defined(linux)
  struct utsname buf;
  int n;

  if (uname(&buf))
    return -1;

  n = strlen(buf.sysname);
  if (namelen <= n)
    n = namelen - 1;
  memcpy(name, buf.sysname, n);
  name[n] = 0;

  n = strlen(buf.release);
  if (releaselen <= n)
    n = releaselen - 1;
  memcpy(release, buf.release, n);
  release[n] = 0;

  return 0;
#else
  return -1;
#endif
}

int
mtl_number_of_processors()
{
/*
#if TS_USE_HWLOC

  int cu = hwloc_get_nbobjs_by_type(ink_get_topology(), HWLOC_OBJ_CORE);

#if HAVE_HWLOC_OBJ_PU
  int pu = hwloc_get_nbobjs_by_type(ink_get_topology(), HWLOC_OBJ_PU);

  if (pu > cu) {
    return cu + (pu - cu)/4;
  }
#endif

  return cu;

#elif defined(freebsd)
*/
#if defined(freebsd)

  int mib[2], n;
  mib[0] = CTL_HW;
  mib[1] = HW_NCPU;
  size_t len = sizeof(n);
  if (sysctl(mib, 2, &n, &len, NULL, 0) == -1)
    return 1;
  return n;

#else

  return sysconf(_SC_NPROCESSORS_ONLN); // number of processing units (includes Hyper Threading)

#endif
}
