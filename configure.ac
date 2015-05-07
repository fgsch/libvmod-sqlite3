AC_INIT([libvmod-sqlite3], [0.1.0])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR(src/vmod.vcc)
AM_CONFIG_HEADER(config.h)

AM_INIT_AUTOMAKE([foreign])

AC_DISABLE_STATIC

AC_PROG_CC
AC_PROG_LIBTOOL

PKG_PROG_PKG_CONFIG

AC_ARG_WITH([system-sqlite],
  [AS_HELP_STRING([--with-system-sqlite],[])],
  [PKG_CHECK_MODULES([SQLITE3], [sqlite3])
   with_system_sqlite=yes],
  [with_system_sqlite=no]
)
AM_CONDITIONAL([WITH_SYSTEM_SQLITE], [test x$with_system_sqlite = xyes])

AC_PATH_PROGS([PYTHON], [python3 python2.7 python2.6])
test -z "$PYTHON" && AC_MSG_ERROR([Python 2.6 or greater is required.])

AC_PATH_PROG([RST2MAN], [rst2man])
test -z "$RST2MAN" && AC_MSG_ERROR([rst2man is required.])

VARNISH_VMOD_INCLUDES
VARNISH_VMOD_DIR
VARNISH_VMODTOOL

AC_PATH_PROG([VARNISHTEST], [varnishtest])
AC_PATH_PROG([VARNISHD], [varnishd])

VMOD_TESTS="$(cd $srcdir/src && echo tests/*.vtc)"
AC_SUBST(VMOD_TESTS)

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
