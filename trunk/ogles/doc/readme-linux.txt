Vincent 3D Rendering Library for Linux (ARM)
============================================

Overview
--------

This file describes howto compile Vincent in Linux environment. The port is mainly intended to 
be used in Nokia 770 but it should also work on Familiar GPE based distributions on IPAQ's or 
ARM desktops running a suitable ARM Linux distribution (for example Debian). The port uses shared 
memory for frame transfers if support for XShm is available. 

Compiling requirements 
----------------------

- native ARM compiling environment 
- Autotools (automake 1.7 and autoconf 2.59 tested)
- GCC (3.4 tested)
- C libraries and headers (glibc 2.3.2 tested)
- X libraries and headers 

The project should also cross compile in Scratchbox (http://www.scratchbox.org/) and test 
programs can be emulated with QEMU (http://fabrice.bellard.free.fr/qemu/). The main development
environment used to create this port is Maemo development platform (http://www.maemo.org, 
uses Scratchbox and qemu-arm for emulation included).

Compiling
---------

- Change directory to project/linux-x11
- run autogen.sh: ./autogen.sh
- run configure: ./configure
  - The default prefix is /usr/local (this can be changed with --prefix=<prefix> option)
- make
- make install

The Vincent can be uninstalled with make uninstall command.

Using Vincent library
---------------------

- Support for pkg-config is included (to add Autotools support)
  - command "pkg-config --libs libvincent" returns required libraries
  - command "pkg-config --cflags libvincent" returns required CFLAGS
- For linking following libraries are needed
  - -dl -lvincent -lpthread -lX11
- If XShm X extension support was enabled following library is needed
  - -lXext

Limitations
-----------

- This port does not support frame buffer devices
- With eglCreateWindowSurface the size is acquired from a X window 
  - X window can be large thus limiting the performance
  - pbuffer surface with eglCopyBuffers can be used if window size is too 
    large (i.e. with window managers that resizes windows to full screen)
- Test programs are not yet included in this port
- eglSaveSurfaceHM extension generates incorrect bitmap
- Support for packaging for Maemo (Debian package format) is missing
