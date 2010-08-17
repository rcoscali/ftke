pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libvincent.pc

includeinstdir = $(includedir)/GLES
includeinst_DATA = \
	$(inc_dir)/GLES/egltypes.h \
	$(inc_dir)/GLES/egl.h \
	$(inc_dir)/GLES/gl.h \
	$(inc_dir)/GLES/glext.h

INCLUDES = -I$(inc_dir) -I$(src_dir) -I$(src_dir)/linux-x11 \
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
        $(src_dir)/linux-x11/ContextLinux.cpp \
        $(src_dir)/linux-x11/Surface.cpp \
        $(src_dir)/linux-x11/Surface.h \
        $(src_dir)/linux-x11/e32def.h \
        $(src_dir)/linux-x11/egl.cpp \
        $(src_dir)/linux-x11/stdafx.cpp \
        $(src_dir)/linux-x11/stdafx.h
