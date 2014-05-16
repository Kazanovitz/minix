# Makefile for Semaphore (SEMA)
PROG=	sema
SRCS=	main.c sema.c queue.c

DPADD+=	${LIBSYS} ${LIBTIMERS}
LDADD+=	-lsys -ltimers

MAN=

BINDIR?= /usr/sbin

CPPFLAGS.main.c+=	-I${NETBSDSRCDIR}
CPPFLAGS.sema.c+=	-I${NETBSDSRCDIR}
CPPFLAGS.queue.c+=	-I${NETBSDSRCDIR}

.include <minix.bootprog.mk>
