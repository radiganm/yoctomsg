## makefile.am
## Copyright 2016 Mac Radigan
## All Rights Reserved

AM_LDFLAGS  = -pthread                                             \
              -L/usr/lib -ldl -lm                                  \
              -L/usr/lib/x86_64-linux-gnu -lgflags                  

AM_CPPFLAGS = -I$(top_srcdir)/include                              \
              -std=c++1z
AM_FCFLAGS  = -Jmod

yocto_ldadd              = libyocto.la                             \
                           -ldl -lm                                \
                           -lrt                                    

yocto_ldflags            = -static-libstdc++                       \
                           -L/usr/lib/x86_64-linux-gnu -lgflags

bin_PROGRAMS             = yoctocat                                \
                           yoctomsg                                \
                           yoctopipe                               \
                           read_channel                            \
                           write_channel                           \
                           basic_channel                           \
                           shmem_channel                           \
                           echo_test

yoctocat_SOURCES         = src/apps/yoctocat.cc
yoctocat_LDADD           = $(yocto_ldadd)
yoctocat_LDFLAGS         = $(yocto_ldflags)

yoctomsg_SOURCES         = src/apps/yoctomsg.cc
yoctomsg_LDADD           = $(yocto_ldadd)
yoctomsg_LDFLAGS         = $(yocto_ldflags)

yoctopipe_SOURCES        = src/apps/yoctopipe.cc
yoctopipe_LDADD          = $(yocto_ldadd)
yoctopipe_LDFLAGS        = $(yocto_ldflags)

read_channel_SOURCES     = src/tests/read_channel.cc
read_channel_LDADD       = $(yocto_ldadd)
read_channel_LDFLAGS     = $(yocto_ldflags)

write_channel_SOURCES    = src/tests/write_channel.cc
write_channel_LDADD      = $(yocto_ldadd)
write_channel_LDFLAGS    = $(yocto_ldflags)

basic_channel_SOURCES    = src/tests/basic_channel.cc
basic_channel_LDADD      = $(yocto_ldadd)
basic_channel_LDFLAGS    = $(yocto_ldflags)

shmem_channel_SOURCES    = src/tests/shmem_channel.cc
shmem_channel_LDADD      = $(yocto_ldadd)
shmem_channel_LDFLAGS    = $(yocto_ldflags)

echo_test_SOURCES        = src/tests/echo_test.cc
echo_test_LDADD          = $(yocto_ldadd)
echo_test_LDFLAGS        = $(yocto_ldflags)

lib_LTLIBRARIES          = libyocto.la
libyocto_la_SOURCES      = src/packages/yocto/Channel.cc                       \
                           src/packages/yocto/BasicChannel.cc                  \
                           src/packages/yocto/ArrayChannel.cc                  \
                           src/packages/yocto/ShmemChannel.cc                  \
                           src/packages/yocto/YoChannel.cc                     \
                           src/packages/yocto/Stream.cc                        \
                           src/packages/yocto/Pipe.cc                          \
                           src/packages/yocto/File.cc                          \
                           src/packages/yocto/AuxFile.cc                       \
                           src/packages/yocto/SharedMemory.cc                   

libyocto_la_LIBADD       = -lz                                                 \
                           -L/usr/lib/x86_64-linux-gnu -lrt

libyocto_la_LDFLAGS      = -export-symbols-regex '^yocto_[^_]' -fPIC           \
                           -shared -pthread                                    \
                           -L/usr/lib/x86_64-linux-gnu -lrt

pkginclude_HEADERS       = include/packages/yocto/Channel.h                    \
                           include/packages/yocto/BasicChannel.h               \
                           include/packages/yocto/ArrayChannel.h               \
                           include/packages/yocto/ShmemChannel.h               \
                           include/packages/yocto/YoChannel.h                  \
                           include/packages/yocto/Stream.h                     \
                           include/packages/yocto/Pipe.h                       \
                           include/packages/yocto/File.h                       \
                           include/packages/yocto/AuxFile.h                    \
                           include/packages/yocto/SharedMemory.h                

## *EOF*
