/* config.h.  Generated from config.h.in by configure.  */
/* config.h.in.  Generated from configure.ac by autoheader.  */

/* Set to the user and host that builds PowerDNS */
#define BUILD_HOST "root@90-20.cachepro.local"

/* Defined if the requested minimum BOOST version is satisfied */
#define HAVE_BOOST 1

/* Defined if the Boost context library is available */
#define HAVE_BOOST_CONTEXT 1

/* Define to 1 if you have <boost/context/detail/fcontext.hpp> */
#define HAVE_BOOST_CONTEXT_DETAIL_FCONTEXT_HPP 1

/* Define to 1 if you have <boost/context/fcontext.hpp> */
/* #undef HAVE_BOOST_CONTEXT_FCONTEXT_HPP */

/* Defined if the Boost system library is available */
#define HAVE_BOOST_SYSTEM 1

/* Define to 1 if you have <boost/system/error_code.hpp> */
#define HAVE_BOOST_SYSTEM_ERROR_CODE_HPP 1

/* Defined if the Boost thread library is available */
#define HAVE_BOOST_THREAD 1

/* Define to 1 if you have <boost/thread.hpp> */
#define HAVE_BOOST_THREAD_HPP 1

/* Define to 1 if you have botan 1.10 */
/* #undef HAVE_BOTAN110 */

/* Define to 1 if you have clock_gettime */
#define HAVE_CLOCK_GETTIME 1

/* define if the compiler supports basic C++11 syntax */
/* #undef HAVE_CXX11 */

/* Define to 1 if you have the declaration of `NID_secp384r1', and to 0 if you
   don't. */
#define HAVE_DECL_NID_SECP384R1 1

/* Define to 1 if you have the declaration of `NID_X9_62_prime256v1', and to 0
   if you don't. */
#define HAVE_DECL_NID_X9_62_PRIME256V1 1

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* define to 1 if OpenSSL ecdsa support is avalable. */
#define HAVE_LIBCRYPTO_ECDSA 1

/* Define to 1 if you have lua */
#define HAVE_LUA 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define if using protobuf. */
/* #undef HAVE_PROTOBUF */

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strcasestr' function. */
#define HAVE_STRCASESTR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Systemd available and enabled */
/* #undef HAVE_SYSTEMD */

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to the sub-directory in which libtool stores uninstalled libraries.
   */
#define LT_OBJDIR ".libs/"

/* Define to 1 if you want to benefit from malloc trace */
/* #undef MALLOC_TRACE */

/* If your OS is so broken that it needs an additional prototype */
/* #undef NEED_INET_NTOP_PROTO */

/* If POSIX typedefs need to be defined */
/* #undef NEED_POSIX_TYPEDEF */

/* Name of package */
#define PACKAGE "pdns-recursor"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "pdns-recursor"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "pdns-recursor 4.0.3"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "pdns-recursor"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "4.0.3"

/* pdns-recursor configure arguments */
#define PDNS_CONFIG_ARGS " 'LDFLAGS=-Wl,-lprofiler,-lcurl'"

/* This is the PowerDNS Recursor */
#define RECURSOR 1

/* Define to 1 for reproducible builds */
/* #undef REPRODUCIBLE */

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if verbose logging is enabled */
/* #undef VERBOSELOG */

/* Version number of package */
#define VERSION "4.0.3"

/* Define _GNU_SOURCE so that we get all necessary prototypes */
#define _GNU_SOURCE 1
