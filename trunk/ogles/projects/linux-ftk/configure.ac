AC_INIT(Makefile.am)
AM_INIT_AUTOMAKE(libvincent, 0.0.1)
AM_CONFIG_HEADER(linux-config.h)

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LIBTOOL

#Check pthreads
AC_MSG_CHECKING([for pthread options])
AC_CHECK_LIB(pthread, pthread_join, PTHREAD_LIBS="-lpthread",
     AC_MSG_ERROR([Could not find pthread_join in -lpthread.]))
AC_CHECK_HEADERS([pthread.h])

WFLAGS="-Wall -Wmissing-prototypes -Wmissing-declarations -Werror -Wcast-align"

CFLAGS="$CFLAGS -DUSE_FTK -g -O2 -Wall `pkg-config --cflags ftk`"
CXXFLAGS="$CFLAGS -DUSE_FTK -g -O2 -Wall `pkg-config --cflags ftk`"
LDFLAGS="$LDFLAGS -lpthread `pkg-config --libs ftk`"


#evil hack, because our makefiles are not on their normal places
src_dir=./../../src
AC_SUBST(src_dir)
inc_dir=./../../include
AC_SUBST(inc_dir)

AC_OUTPUT(Makefile \
          libvincent.pc )
