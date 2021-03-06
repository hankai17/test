# $Id$

SHELL=/bin/sh
INSTALL=/usr/bin/install -c
RANLIB=ranlib

srcdir=.
top_srcdir=.
prefix=/usr/local
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include
mandir=${prefix}/share/man

LIB=libcares.a
CC=gcc
CPPFLAGS=
CFLAGS=-g -O2 ${WARN_CFLAGS} ${ERROR_CFLAGS}
DEFS=-DPACKAGE_NAME=\"\" -DPACKAGE_TARNAME=\"\" -DPACKAGE_VERSION=\"\" -DPACKAGE_STRING=\"\" -DPACKAGE_BUGREPORT=\"\" -DPACKAGE_URL=\"\"
LDFLAGS=
LIBS=
ALL_CFLAGS=${CPPFLAGS} ${CFLAGS} ${DEFS}
OBJS=	ares__close_sockets.o ares__get_hostent.o ares__read_line.o \
	ares_destroy.o ares_expand_name.o ares_fds.o \
	ares_free_hostent.o ares_free_string.o ares_gethostbyaddr.o \
	ares_gethostbyname.o ares_init.o ares_mkquery.o ares_parse_a_reply.o \
	ares_parse_ptr_reply.o ares_process.o ares_query.o ares_search.o \
	ares_send.o ares_strerror.o ares_timeout.o ares_version.o

$(LIB): ${OBJS}
	ar cru $@ ${OBJS}
	${RANLIB} $@

all: $(LIB) demos

demos: adig ahost

tags:
	etags *.[ch]

adig: adig.o $(LIB)
	${CC} ${LDFLAGS} -o $@ adig.o $(LIB) ${LIBS}

ahost: ahost.o $(LIB)
	${CC} ${LDFLAGS} -o $@ ahost.o $(LIB) ${LIBS}

${OBJS}: ares.h ares_dns.h ares_private.h

.c.o:
	${CC} -c ${ALL_CFLAGS} $<

check:

install:
	${top_srcdir}/mkinstalldirs ${DESTDIR}${libdir}
	${top_srcdir}/mkinstalldirs ${DESTDIR}${includedir}
	${top_srcdir}/mkinstalldirs ${DESTDIR}${mandir}/man3
	${INSTALL} -m 644 $(LIB) ${DESTDIR}${libdir}
	${RANLIB} ${DESTDIR}${libdir}/$(LIB)
	chmod u-w ${DESTDIR}${libdir}/$(LIB)
	${INSTALL} -m 444 ${srcdir}/ares.h ${DESTDIR}${includedir}
	${INSTALL} -m 444 ${srcdir}/ares_version.h ${DESTDIR}${includedir}
	${INSTALL} -m 444 ${srcdir}/ares_destroy.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_expand_name.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_fds.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_free_hostent.3 \
		${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_free_string.3 \
		${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_gethostbyaddr.3 \
		${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_gethostbyname.3 \
		${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_init.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_init_options.3 \
		${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_mkquery.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_parse_a_reply.3 \
		${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_parse_ptr_reply.3 \
		${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_process.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_query.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_search.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_send.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_strerror.3 ${DESTDIR}${mandir}/man3
	${INSTALL} -m 444 ${srcdir}/ares_timeout.3 ${DESTDIR}${mandir}/man3

clean:
	rm -f ${OBJS} $(LIB) adig.o adig ahost.o ahost

distclean: clean
	rm -f config.cache config.log config.status Makefile
