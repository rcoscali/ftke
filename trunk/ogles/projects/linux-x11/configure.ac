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

#Check x11
AC_MSG_CHECKING([for X11 options])
AC_CHECK_LIB(X11, XOpenDisplay, X11_LIBS="-lX11 -dl", 
     AC_MSG_ERROR([Could not find XOpenDisplay in -lX11.]))
AC_CHECK_HEADERS([X11/Xlib.h])

#Check xshm (optional, better performance)
AC_CHECK_HEADERS([X11/extensions/XShm.h], [h=yes], [h=no], [#include <X11/Xlib.h>])
AC_MSG_CHECKING([for XShm options])
AC_CHECK_LIB(Xext, XShmQueryVersion, XSHM_LIBS="-lXext", 
     AC_MSG_ERROR([Could not find XShmQueryVersion in -lXext.]))

#Check xsp (optional, for pixel doubling), in application or in vincent?
#AC_MSG_CHECKING([for Xsp options])
#AC_CHECK_LIB(Xsp, XSPSetPixelDoubling, XSP_LIBS="-lXsp", 
#     AC_MSG_ERROR([Could not find XSPSetPixelDoubling in -lX11.]))
#AC_CHECK_HEADERS([X11/extensions/Xsp.h])
#AC_MSG_RESULT([yes])

#check for gpp, for Intel processors, not yet present

X11_CFLAGS="-D_XOPEN_SOURCE=500 -I/usr/X11R6/include -DXTHREADS -DXUSE_MTSAFE_API"

WFLAGS="-Wall -Wmissing-prototypes -Wmissing-declarations -Werror -Wcast-align"
CFLAGS="-g -02 -Wall $X11_CFLAGS"

#For Xsp
#LIBS="$PTHREAD_LIBS $XSP_LIBS $XSHM_LIBS $X11_LIBS"
LIBS="$PTHREAD_LIBS $XSHM_LIBS $X11_LIBS"

#evil hack, because our makefiles are not on their normal places
src_dir=./../../src
AC_SUBST(src_dir)
inc_dir=./../../include
AC_SUBST(inc_dir)

AC_OUTPUT(Makefile \
          libvincent.pc )
