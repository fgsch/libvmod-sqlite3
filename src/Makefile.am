
vmoddir = $(VMODDIR)
vmod_LTLIBRARIES = libvmod_sqlite3.la

libvmod_sqlite3_la_CFLAGS = -I$(VARNISHSRC)/include -I$(VARNISHSRC) \
	$(SQLITE3_CFLAGS)
libvmod_sqlite3_la_LDFLAGS = -module -export-dynamic -avoid-version -shared \
	$(SQLITE3_LIBS)

libvmod_sqlite3_la_SOURCES = \
	vmod_sqlite3.c
if !WITH_SYSTEM_SQLITE
libvmod_sqlite3_la_CFLAGS += -I$(top_srcdir)/sqlite3
libvmod_sqlite3_la_SOURCES += \
	$(top_srcdir)/sqlite3/sqlite3.c \
	$(top_srcdir)/sqlite3/sqlite3.h \
	$(top_srcdir)/sqlite3/sqlite3ext.h
endif
nodist_libvmod_sqlite3_la_SOURCES = \
	vcc_if.c \
	vcc_if.h

VMOD_TESTS = tests/*.vtc

$(libvmod_sqlite3_la_OBJECTS): vcc_if.h

vcc_if.c vcc_if.h: $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod.vcc
	$(PYTHON) $(VARNISHSRC)/lib/libvmod_std/vmod.py $(top_srcdir)/src/vmod.vcc

check:
	$(VARNISHSRC)/bin/varnishtest/varnishtest \
	    -Dvarnishd=$(VARNISHSRC)/bin/varnishd/varnishd \
	    -Dvmod_topbuild=$(abs_top_builddir) $(VMOD_TESTS)

EXTRA_DIST = vmod.vcc \
	$(VMOD_TESTS)

CLEANFILES = $(builddir)/vcc_if.c \
	$(builddir)/vcc_if.h
