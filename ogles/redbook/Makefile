CFILES = \
	aargb.c \
	alpha3D.c \
	alpha.c \
	checker.c \
	colormat.c \
	cube.c \
	double.c \
	fog.c \
	hello.c \
	light.c \
	lines.c \
	material.c \
	mipmap.c \
	model.c \
	movelight.c \
	planet.c \
	polyoff.c \
	robot.c \
	scene.c \
	smooth.c \
	texbind.c \
	texsub.c \
	torus.c \
	varray.c \
	wrap.c

LLDLIBS = -L.. -lug -L../../.. -lGLES_CL -L/usr/X11R6/lib 
CFLAGS = -I../include -I../gu
LDFLAGS = $(LLDLIBS)

TARGETS = $(subst .c,.exe,$(CFILES))

default apps: $(TARGETS)

%.exe: %.c
	$(CC) $(CFLAGS) -o $@ $@.c $(LDFLAGS)

linux:
	gcc -g hello.c -o t -I../include -I../src  -L ../projects/linux-x11/.libs/ -lvincent -lstdc++ -lX11 -lpthread

