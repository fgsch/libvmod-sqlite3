===============
libvmod-sqlite3
===============

----------------------
Varnish SQLite3 Module
----------------------

SYNOPSIS
========
import sqlite3;

DESCRIPTION
===========
`libvmod-sqlite3` allow you to integrate Varnish with SQLite3 Database.

To enable it you need to import this *vmod*(sqlite3).
Open the SQLite database specified by the 1st argument and set
the field and row delimiters to be used in `exec()` to the 1st
and 2nd character, respectively, of the 2nd argument::
    sub vcl_init {
        sqlite3.open("mydata.db", "|;");
    }

And that's all. You can INSERT,DELETE,UPDATE data in db.

CONTENTS
========

.. _obj_exec:
exec
-----

Description
	Run an SQL statements and return any result rows coming out
	of their evaluation.  If an error occurs return the error text
	instead.
Example
	set req.http.results = sqlite3.exec("SELECT foo FROM bar");


.. _obj_escape:
escape
------
Description
	Escape the input string by doubling any single-quote (')
	character and return the result.
Example
	    set req.http.url = sqlite3.escape(req.url);

.. _obj_close:
close
-----
Description
	Close a previously opened SQLite database.
Example
	sqlite3.close()

INSTALLATION
============

The source tree is based on autotools to configure the building, and
does also have the necessary bits in place to do functional unit tests
using the ``varnishtest`` tool.

Building requires the Varnish header files and uses pkg-config to find
the necessary paths.

Usage::

 ./autogen.sh
 ./configure

If you have installed Varnish to a non-standard directory, call
``autogen.sh`` and ``configure`` with ``PKG_CONFIG_PATH`` pointing to
the appropriate path. For instance, when varnishd configure was called
with ``--prefix=$PREFIX``, use

::

 export PKG_CONFIG_PATH=${PREFIX}/lib/pkgconfig
 export ACLOCAL_PATH=${PREFIX}/share/aclocal

The module will inherit its prefix from Varnish, unless you specify a
different ``--prefix`` when running the ``configure`` script for this
module.

Make targets:

* make - builds the vmod.
* make install - installs your vmod.
* make check - runs the unit tests in ``src/tests/*.vtc``.
* make distcheck - run check and prepare a tarball of the vmod.

If you build a dist tarball, you don't need any of the autotools or
pkg-config. You can build the module simply by running::

 ./configure
 make

Installation directories
------------------------

By default, the vmod ``configure`` script installs the built vmod in the
directory relevant to the prefix. The vmod installation directory can be
overridden by passing the ``vmoddir`` variable to ``make install``.

USAGE
=====

In your VCL you could then use this vmod along the following lines::

        import sqlite3;

        sub vcl_init {
            sqlite3.open("mydata.db", "|;");
        }

        sub vcl_recv {
            if (req.url ~ "^/add") {
                set req.http.r = sqlite3.exec(
                    "INSERT INTO TEST VALUES(1, 2)");
            } else if (req.url ~ "^/new") {
                set req.http.r = sqlite3.exec(
                    "CREATE TABLE TEST(a INTEGER, b INTEGER)");
            } else if (req.url ~ "^/list") {
                set req.http.r = sqlite3.exec(
                    "SELECT a, b FROM TEST");
            } else if (req.url ~ "^/close") {
                sqlite3.close();
            }
            return (pass);
    	}

COMMON PROBLEMS
===============

* configure: error: Need varnish.m4 -- see README.rst

  Check whether ``PKG_CONFIG_PATH`` and ``ACLOCAL_PATH`` were set correctly
  before calling ``autogen.sh`` and ``configure``

* Incompatibilities with different Varnish Cache versions

  Make sure you build this vmod against its correspondent Varnish Cache version.
  For instance, to build against Varnish Cache 6.0, this vmod must be built from
  branch 6.0.

Stuff
=====

[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/fgsch/libvmod-sqlite3?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/fgsch/libvmod-sqlite3.svg?branch=master)](https://travis-ci.org/fgsch/libvmod-sqlite3)