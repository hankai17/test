#ifndef _mtl_platform_hh
#define _mtl_platform_hh

//#include "ink_config.h"

#include <stdio.h>
#include <stdarg.h>
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#include <ctype.h>
#ifdef HAVE_STRING_H
# include <string.h>
#endif
#ifdef HAVE_STRINGS_H
# include <strings.h>
#endif
#include <errno.h>
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#include <fcntl.h>

#include <limits.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#include <sys/stat.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <sys/file.h>
#include <sys/resource.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <sys/param.h>
#include <sys/un.h>

#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/mman.h>

#ifdef HAVE_NETINET_IN_H
# include <netinet/in.h>
#endif
#ifdef HAVE_NETINET_IN_SYSTM_H
# include <netinet/in_systm.h>
#endif
#ifdef HAVE_NETINET_TCP_H
# include <netinet/tcp.h>
#endif
#ifdef HAVE_NETINET_IP_H
# include <netinet/ip.h>
#endif
#ifdef HAVE_NETINET_IP_ICMP_H
# include <netinet/ip_icmp.h>
#endif
#ifdef HAVE_NETDB_H
# include <netdb.h>
#endif
#ifdef HAVE_ARPA_INET_H
# include <arpa/inet.h>
#endif
#ifdef HAVE_ARPA_NAMESER_H
# include <arpa/nameser.h>
#endif
#ifdef HAVE_ARPA_NAMESER_COMPAT_H
# include <arpa/nameser_compat.h>
#endif

#include <signal.h>
#ifdef HAVE_SIGINFO_H
# include <siginfo.h>
#endif
#ifdef HAVE_WAIT_H
# include <wait.h>
#endif

#include <syslog.h>
#include <pwd.h>
#include <poll.h>

#if TS_USE_EPOLL
#include <sys/epoll.h>
#endif
#if TS_USE_KQUEUE
#include <sys/event.h>
#endif
#if TS_USE_PORT
#include <port.h>
#endif


#ifdef HAVE_VALUES_H
# include <values.h>
#endif
#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#endif

#include <errno.h>
#include <dirent.h>

#ifdef HAVE_CPIO_H
# include <cpio.h>
#endif

#include <net/if.h>

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif

// Gnu C++ doesn't define __STDC__ == 0 as needed to have ip_hl be defined
#if defined(__GNUC__) && !defined(__STDC__)
#define __STDC__ 0
#endif

#ifdef HAVE_MACHINE_ENDIAN_H
# include <machine/endian.h>
#endif
#ifdef HAVE_ENDIAN_H
# include <endian.h>
#endif
#ifdef HAVE_SYS_BYTEORDER_H
# include <sys/byteorder.h>
#endif

#ifdef HAVE_SYS_IOCTL_H
# include <sys/ioctl.h>
#endif
#ifdef HAVE_SYS_SOCKIO_H
# include <sys/sockio.h>
#endif

#include <resolv.h>


#if defined(linux) //linux os
typedef unsigned int in_addr_t; //一般为32位的unsigned int 其字节顺序为网络顺序network byte ordered 即该无符号整数采用大端字节序
#endif

#ifdef HAVE_SYS_SYSINFO_H
# include <sys/sysinfo.h>
#endif

#if !defined(darwin)
# ifdef HAVE_SYS_SYSCTL_H
#  include <sys/sysctl.h>
# endif
#endif
#ifdef HAVE_SYS_SYSTEMINFO_H
# include <sys/systeminfo.h>
#endif

#ifdef HAVE_DLFCN_H
# include <dlfcn.h>
#endif
#ifdef HAVE_MATH_H
# include <math.h>
#endif
#ifdef HAVE_FLOAT_H
# include <float.h>
#endif

#ifdef HAVE_SYS_SYSMACROS_H
# include <sys/sysmacros.h>
#endif

#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#endif


#ifndef PATH_NAME_MAX
#define PATH_NAME_MAX 4096 // instead of PATH_MAX which is inconsistent
                           // on various OSs (linux-4096,osx/bsd-1024,
                           //                 windows-260,etc)
#endif

#endif /* _mtl_platform_h */
