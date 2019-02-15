AM_CFLAGS = $(VARNISHAPI_CFLAGS) -Wall -Werror
AM_LDFLAGS = $(VARNISHAPI_LIBS) $(VMOD_LDFLAGS)

vmod_LTLIBRARIES = libvmod_sqlite3.la

libvmod_sqlite3_la_SOURCES = \
	vmod_sqlite3.c \
	sqlite3.h \
	sqlite3.c \
	sqlite3exc.h

nodist_libvmod_sqlite3_la_SOURCES = \
	vcc_sqlite3_if.c \
	vcc_sqlite3_if.h

dist_man_MANS = vmod_sqlite3.3

@BUILD_VMOD_SQLITE3@

AM_TESTS_ENVIRONMENT = \
	PATH="$(VMOD_TEST_PATH)" \
	LD_LIBRARY_PATH="$(VARNISH_LIBRARY_PATH)"
TEST_EXTENSIONS = .vtc
VTC_LOG_COMPILER = varnishtest -v
AM_VTC_LOG_FLAGS = -Dvmod_sqlite3="$(VMOD_SQLITE3)"

TESTS = @VMOD_TESTS@

EXTRA_DIST = \
	vmod_sqlite3.vcc \
	$(VMOD_TESTS)
