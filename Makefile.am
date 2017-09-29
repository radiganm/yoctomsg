## makefile.am
## Copyright 2016 Mac Radigan
## All Rights Reserved

AM_LDFLAGS  = -pthread                                      \
              -L/usr/lib -ldl -lm                            

AM_CPPFLAGS = -I$(top_srcdir)/include                       \
              -std=c++11
AM_FCFLAGS  = -Jmod

yocto_ldadd              = libyocto.la                      \
                           -ldl -lm                         \
                           -lrt                              

bin_PROGRAMS             = test_channel                      

test_channel_SOURCES     = src/tests/test_channel.cc
test_channel_LDADD       = $(yocto_ldadd)

lib_LTLIBRARIES          = libyocto.la
libyocto_la_SOURCES      = src/packages/yocto/Channel.cc                        

libyocto_la_LIBADD         = -lz                                               \
                           -L/usr/lib/x86_64-linux-gnu -lrt

libyocto_la_LDFLAGS        = -export-symbols-regex '^yocto_[^_]' -fPIC         \
                           -shared -pthread                                    \
                           -L/usr/lib/x86_64-linux-gnu -lrt

pkginclude_HEADERS       = include/packages/yocto/Channel.h                     

## *EOF*
