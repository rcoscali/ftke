pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libvincent.pc

includeinstdir = $(includedir)/GLES
includeinst_DATA = \
	$(inc_dir)/GLES/egltypes.h \
	$(inc_dir)/GLES/egl.h \
	$(inc_dir)/GLES/gl.h \
	$(inc_dir)/GLES/glext.h

INCLUDES = -I$(inc_dir) -I$(src_dir) -I$(src_dir)/linux-ftk \
	-I$(src_dir)/arm -I$(src_dir)/codegen

lib_LTLIBRARIES = libvincent.la
libvincent_la_SOURCES =  \
        $(src_dir)/Arrays.h \
        $(src_dir)/Buffer.h \
        $(src_dir)/Color.h \
        $(src_dir)/Config.cpp \
        $(src_dir)/Config.h \
        $(src_dir)/Context.cpp \
        $(src_dir)/Context.h \
        $(src_dir)/ContextBuffer.cpp \
        $(src_dir)/ContextFloat.cpp \
        $(src_dir)/ContextLight.cpp \
        $(src_dir)/ContextLines.cpp \
        $(src_dir)/ContextMaterial.cpp \
        $(src_dir)/ContextMatrix.cpp \
        $(src_dir)/ContextMesh.cpp \
        $(src_dir)/ContextPoints.cpp \
        $(src_dir)/ContextRaster.cpp \
        $(src_dir)/ContextRender.cpp \
        $(src_dir)/ContextTexture.cpp \
        $(src_dir)/ContextTriangles.cpp \
        $(src_dir)/ContextViewport.cpp \
        $(src_dir)/Display.cpp \
        $(src_dir)/Display.h \
        $(src_dir)/FractionalColor.h \
        $(src_dir)/Light.cpp \
        $(src_dir)/Light.h \
        $(src_dir)/LineClipper.inc \
        $(src_dir)/Material.cpp \
        $(src_dir)/Material.h \
        $(src_dir)/MatrixStack.cpp \
        $(src_dir)/MatrixStack.h \
        $(src_dir)/OGLES.h \
        $(src_dir)/Rasterizer.cpp \
        $(src_dir)/Rasterizer.h \
        $(src_dir)/RasterizerState.cpp \
        $(src_dir)/RasterizerState.h \
        $(src_dir)/RasterizerTriangles.cpp \
        $(src_dir)/RasterizerTriangles.inc \
        $(src_dir)/Texture.cpp \
        $(src_dir)/Texture.h \
        $(src_dir)/TriangleClipper.inc \
        $(src_dir)/TriangleClipperHigh.inc \
        $(src_dir)/TriangleClipperLow.inc \
        $(src_dir)/Types.h \
        $(src_dir)/Utils.cpp \
        $(src_dir)/Utils.h \
        $(src_dir)/fixed.cpp \
        $(src_dir)/fixed.h \
        $(src_dir)/gl.cpp \
        $(src_dir)/gl_sin.h \
        $(src_dir)/linalg.cpp \
        $(src_dir)/linalg.h \
        $(src_dir)/arm/CodeGenerator.cpp \
        $(src_dir)/arm/CodeGenerator.h \
        $(src_dir)/arm/FunctionCache.cpp \
        $(src_dir)/arm/FunctionCache.h \
        $(src_dir)/arm/GenFragment.cpp \
        $(src_dir)/arm/GenLline.cpp \
        $(src_dir)/arm/GenPoint.cpp \
        $(src_dir)/arm/GenScanline.cpp \
        $(src_dir)/arm/GenTriangle.cpp \
        $(src_dir)/codegen/arm-codegen.c \
        $(src_dir)/codegen/arm-codegen.h \
        $(src_dir)/codegen/arm-dis.c \
        $(src_dir)/codegen/arm-dis.h \
        $(src_dir)/codegen/arm_dpimacros.h \
        $(src_dir)/codegen/bitset.c \
        $(src_dir)/codegen/bitset.h \
        $(src_dir)/codegen/codegen.h \
        $(src_dir)/codegen/emit.c \
        $(src_dir)/codegen/emit.h \
        $(src_dir)/codegen/heap.c \
        $(src_dir)/codegen/heap.h \
        $(src_dir)/codegen/instruction.c \
        $(src_dir)/codegen/instruction.h \
        $(src_dir)/codegen/segment.c \
        $(src_dir)/codegen/segment.h \
        $(src_dir)/linux-ftk/ContextLinux.cpp \
        $(src_dir)/linux-ftk/Surface.cpp \
        $(src_dir)/linux-ftk/Surface.h \
        $(src_dir)/linux-ftk/e32def.h \
        $(src_dir)/linux-ftk/egl.cpp \
        $(src_dir)/linux-ftk/stdafx.cpp \
        $(src_dir)/linux-ftk/stdafx.h \
        $(src_dir)/../ug/ug_ftk.c  \
        $(src_dir)/../ug/uglu.c \
        $(src_dir)/../ug/ugshape.c \
        $(src_dir)/../ug/gl_sin.h

noinst_PROGRAMS=aargb  alpha3D  alpha  checker  colormat  cube  double  fog  hello  light  lines  material  mipmap  model  movelight  planet  polyoff  robot  scene  smooth  texbind  texsub  torus  varray  wrap

aargb_SOURCES=$(src_dir)/../redbook/aargb.c
aargb_LDFLAGS=libvincent.la
alpha3D_SOURCES=$(src_dir)/../redbook/alpha3D.c
alpha3D_LDFLAGS=libvincent.la
alpha_SOURCES=$(src_dir)/../redbook/alpha.c
alpha_LDFLAGS=libvincent.la
checker_SOURCES=$(src_dir)/../redbook/checker.c
checker_LDFLAGS=libvincent.la
colormat_SOURCES=$(src_dir)/../redbook/colormat.c
colormat_LDFLAGS=libvincent.la
cube_SOURCES=$(src_dir)/../redbook/cube.c
cube_LDFLAGS=libvincent.la
double_SOURCES=$(src_dir)/../redbook/double.c
double_LDFLAGS=libvincent.la
fog_SOURCES=$(src_dir)/../redbook/fog.c
fog_LDFLAGS=libvincent.la
hello_SOURCES=$(src_dir)/../redbook/hello.c
hello_LDFLAGS=libvincent.la
light_SOURCES=$(src_dir)/../redbook/light.c
light_LDFLAGS=libvincent.la
lines_SOURCES=$(src_dir)/../redbook/lines.c
lines_LDFLAGS=libvincent.la
material_SOURCES=$(src_dir)/../redbook/material.c
material_LDFLAGS=libvincent.la
mipmap_SOURCES=$(src_dir)/../redbook/mipmap.c
mipmap_LDFLAGS=libvincent.la
model_SOURCES=$(src_dir)/../redbook/model.c
model_LDFLAGS=libvincent.la
movelight_SOURCES=$(src_dir)/../redbook/movelight.c
movelight_LDFLAGS=libvincent.la
planet_SOURCES=$(src_dir)/../redbook/planet.c
planet_LDFLAGS=libvincent.la
polyoff_SOURCES=$(src_dir)/../redbook/polyoff.c
polyoff_LDFLAGS=libvincent.la
robot_SOURCES=$(src_dir)/../redbook/robot.c
robot_LDFLAGS=libvincent.la
scene_SOURCES=$(src_dir)/../redbook/scene.c
scene_LDFLAGS=libvincent.la
smooth_SOURCES=$(src_dir)/../redbook/smooth.c
smooth_LDFLAGS=libvincent.la
texbind_SOURCES=$(src_dir)/../redbook/texbind.c
texbind_LDFLAGS=libvincent.la
texsub_SOURCES=$(src_dir)/../redbook/texsub.c
texsub_LDFLAGS=libvincent.la
torus_SOURCES=$(src_dir)/../redbook/torus.c
torus_LDFLAGS=libvincent.la
varray_SOURCES=$(src_dir)/../redbook/varray.c
varray_LDFLAGS=libvincent.la
wrap_SOURCES=$(src_dir)/../redbook/wrap.c
wrap_LDFLAGS=libvincent.la

