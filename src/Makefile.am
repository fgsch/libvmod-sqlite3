
vmoddir = $(VMOD_DIR)
vmod_LTLIBRARIES = libvmod_sqlite3.la

libvmod_sqlite3_la_CFLAGS = $(VMOD_INCLUDES) $(SQLITE3_CFLAGS)
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

dist_man_MANS = vmod_sqlite3.3

VTC_LOG_COMPILER = $(VARNISHTEST) -v -i \
	-Dvarnishd=$(VARNISHD) -Dvmod_topbuild=$(abs_top_builddir)
TEST_EXTENSIONS = .vtc
TESTS = @VMOD_TESTS@

$(libvmod_sqlite3_la_OBJECTS): vcc_if.h

vcc_if.c vcc_if.h vmod_sqlite3.man.rst: $(VMODTOOL) $(top_srcdir)/src/vmod.vcc
	$(PYTHON) $(VMODTOOL) $(top_srcdir)/src/vmod.vcc

vmod_sqlite3.3: vmod_sqlite3.man.rst
	${RST2MAN} $? $@

EXTRA_DIST = vmod.vcc \
	$(TESTS)

CLEANFILES = $(builddir)/vcc_if.c \
	$(builddir)/vcc_if.h \
	$(builddir)/vmod_sqlite3.man.rst \
	$(builddir)/vmod_sqlite3.rst \
	$(builddir)/$(dist_man_MANS)
