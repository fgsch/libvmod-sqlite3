libvmod-sqlite3
===============

[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/fgsch/libvmod-sqlite3?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Build Status](https://travis-ci.org/fgsch/libvmod-sqlite3.svg?branch=master)](https://travis-ci.org/fgsch/libvmod-sqlite3)

Installation
------------

Ubuntu Trusty (14.04):

    apt-get install install autoconf libvarnishapi-dev python-docutils libtool
    ./autogen.sh
    ./configure
    make CFLAGS="-Wall -Wextra -Werror"
    make check
    make install
