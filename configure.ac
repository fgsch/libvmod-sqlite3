AC_INIT([libvmod-sqlite3], [0.1])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR(src/vmod.vcc)
AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CC
AC_PROG_LIBTOOL

PKG_PROG_PKG_CONFIG
PKG_CHECK_MODULES([SQLITE3], [sqlite3])

AC_PATH_PROGS([PYTHON], [python3 python2.7 python2.6])
test -z "$PYTHON" && AC_MSG_ERROR([Python 2.6 or greater is required.])

AC_PATH_PROG([RST2MAN], [rst2man])
test -z "$RST2MAN" && AC_MSG_ERROR([rst2man is required.])

VARNISH_VMOD_INCLUDES
VARNISH_VMOD_DIR
VARNISH_VMODTOOL

AC_PATH_PROG([VARNISHTEST], [varnishtest])
AC_PATH_PROG([VARNISHD], [varnishd])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
