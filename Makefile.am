## makefile.am
## Copyright 2016 Mac Radigan
## All Rights Reserved

AM_LDFLAGS  = -pthread                                      \
              -L/usr/lib -ldl -lm                            

AM_CPPFLAGS = -I$(top_srcdir)/include                       \
AM_FCFLAGS  = -Jmod

yocto_ldadd              = libyocto.la                      \
                           -ldl -lm                         \
                           -lrt                              

bin_PROGRAMS             = test_client                      \
                           test_server                       

test_client_SOURCES      = src/tests/test_oscilloscope_client.cc
test_client_LDADD        = $(yocto_ldadd)

test_server_SOURCES      = src/tests/test_surf.cc
test_server_LDADD        = $(yocto_ldadd)

lib_LTLIBRARIES          = libyocto.la
libyocto_la_SOURCES      = src/packages/common/status.cc                       \
                           src/packages/channel/Channel.cc                     \
                           src/packages/test/test.cc                            

libyocto_la_LIBADD         = -lz                                               \
                           -L/usr/lib/x86_64-linux-gnu -lrt

libyocto_la_LDFLAGS        = -export-symbols-regex '^yocto_[^_]' -fPIC         \
                           -shared -pthread                                    \
                           -L/usr/lib/x86_64-linux-gnu -lrt

pkginclude_HEADERS       = include/packages/common/status.h                    \
                           include/packages/common/debug.h                     \
                           include/packages/channel/Channel.h                  \
                           include/packages/test/test.h                         

## *EOF*
