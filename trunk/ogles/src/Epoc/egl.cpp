// ==========================================================================
//
// gl.cpp	EGL Client API entry points
//
// --------------------------------------------------------------------------
//
// 08-04-2003	Hans-Martin Will	initial version
//
// --------------------------------------------------------------------------
//
// Copyright (c) 2004, Hans-Martin Will. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//	 *  Redistributions of source code must retain the above copyright
// 		notice, this list of conditions and the following disclaimer.
//   *	Redistributions in binary form must reproduce the above copyright
// 		notice, this list of conditions and the following disclaimer in the
// 		documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
// OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
// THE POSSIBILITY OF SUCH DAMAGE.
//
// ==========================================================================


#include "stdafx.h"
#include "GLES/gl.h"
#include "GLES/egl.h"
#include "Context.h"
#include "Rasterizer.h"
#include "Config.h"
#include "Surface.h"
#include <w32std.h>


using namespace EGL;


// version numbers
#define EGL_VERSION_MAJOR 1
#define EGL_VERSION_MINOR 0

extern "C" {

	GLAPI EGLBoolean APIENTRY eglInitialize (EGLDisplay dpy, EGLint *major, EGLint *minor) {

		if (major != 0) {
			*major = EGL_VERSION_MAJOR;
		}

		if (minor != 0) {
			*minor = EGL_VERSION_MINOR;
		}

		// allocate thread local storage
		TlsInfo * info = reinterpret_cast<TlsInfo *>(Dll::Tls());

		if (!info) {
			info = new TlsInfo();
			Dll::SetTls(info);
		}

		return TRUE;
	}


	GLAPI EGLBoolean APIENTRY eglTerminate (EGLDisplay dpy) {

		TlsInfo * info = reinterpret_cast<TlsInfo *>(Dll::Tls());

		if (info) {
			delete info;
			Dll::SetTls(0);
		}

		return EGL_TRUE;
	}


	static void eglRecordError(EGLint error)
		// Save an error code for the current thread
		//
		// error		-		The error code to be recorded in thread local storage
	{
		TlsInfo * info = reinterpret_cast<TlsInfo *>(Dll::Tls());

		if (info) {
			info->m_LastError = error;
		}
	}


	GLAPI EGLDisplay APIENTRY eglGetDisplay (NativeDisplayType display) {
		return reinterpret_cast<EGLDisplay>(display);
	}


	GLAPI const char * APIENTRY eglQueryString (EGLDisplay dpy, EGLint name) {
		switch (name) {
		case EGL_VENDOR:
			return EGL_CONFIG_VENDOR;

		case EGL_VERSION:
			return EGL_VERSION_NUMBER;

		case EGL_EXTENSIONS:
			return "";

		default:
			eglRecordError(EGL_BAD_PARAMETER);
			return 0;
		}
	}


	static const struct {
		const char * name;
		void * ptr;

	} FunctionTable[] = {
		{ "glQueryMatrixxOES", glQueryMatrixxOES },
	};


	GLAPI void (* APIENTRY eglGetProcAddress (const char *procname))() {

		if (!procname) {
			return 0;
		}

		size_t functions = sizeof(FunctionTable) / sizeof(FunctionTable[0]);

		for (size_t index = 0; index < functions; ++index) {
			if (!strcmp(FunctionTable[index].name, procname))
				return (void (APIENTRY *)(void)) FunctionTable[index].ptr;
		}

		return 0;
	}


	GLAPI EGLBoolean APIENTRY eglGetConfigs (EGLDisplay dpy, EGLConfig *configs, EGLint config_size, EGLint *num_config) {
		return Config::GetConfigs(configs, config_size, num_config);
	}


	GLAPI EGLBoolean APIENTRY eglChooseConfig (EGLDisplay dpy, const EGLint *attrib_list, EGLConfig *configs, EGLint config_size, EGLint *num_config) {
		return Config::ChooseConfig(attrib_list, configs, config_size, num_config);
	}


	GLAPI EGLBoolean APIENTRY eglGetConfigAttrib (EGLDisplay dpy, EGLConfig config, EGLint attribute, EGLint *value) {
		*value = config->GetConfigAttrib(attribute);
		return EGL_TRUE;
	}


	GLAPI EGLSurface APIENTRY eglCreateWindowSurface (EGLDisplay dpy, EGLConfig config, RWindow * window, const EGLint *attrib_list) {

		TSize size = window->Size();

		Config surfaceConfig(*config);
		surfaceConfig.SetConfigAttrib(EGL_SURFACE_TYPE, EGL_WINDOW_BIT);
		surfaceConfig.SetConfigAttrib(EGL_WIDTH, size.iWidth);
		surfaceConfig.SetConfigAttrib(EGL_HEIGHT, size.iHeight);

		return EGL::Surface::NewL(surfaceConfig);
	}


	GLAPI EGLSurface APIENTRY eglCreatePixmapSurface (EGLDisplay dpy, EGLConfig config, NativePixmapType pixmap, const EGLint *attrib_list) {
		// Cannot support rendering to arbitrary native surfaces; use pbuffer surface and eglCopySurfaces instead
		return EGL_NO_SURFACE;
	}


	GLAPI EGLSurface APIENTRY eglCreatePbufferSurface (EGLDisplay dpy, EGLConfig config, const EGLint *attrib_list) {

		static const EGLint validAttributes[] = {
			EGL_WIDTH,
			EGL_HEIGHT,
			EGL_NONE
		};

		Config surfaceConfig(*config, attrib_list, validAttributes);
		surfaceConfig.SetConfigAttrib(EGL_SURFACE_TYPE, EGL_PBUFFER_BIT);

		return EGL::Surface::NewL(surfaceConfig);
	}


	GLAPI EGLBoolean APIENTRY eglDestroySurface (EGLDisplay dpy, EGLSurface surface) {
		surface->Dispose();
		return EGL_TRUE;
	}


	GLAPI EGLBoolean APIENTRY eglQuerySurface (EGLDisplay dpy, EGLSurface surface, EGLint attribute, EGLint *value) {
		*value = surface->GetConfig()->GetConfigAttrib(attribute);
		return EGL_TRUE;
	}


	GLAPI EGLContext APIENTRY eglCreateContext (EGLDisplay dpy, EGLConfig config, EGLContext share_list, const EGLint *attrib_list) {
		return new Context(*config);
	}


	GLAPI EGLBoolean APIENTRY eglDestroyContext (EGLDisplay dpy, EGLContext ctx) {
		ctx->Dispose();
		return EGL_TRUE;
	}


	GLAPI EGLBoolean APIENTRY eglMakeCurrent (EGLDisplay dpy, EGLSurface draw, EGLSurface read, EGLContext ctx) {

		Context::SetCurrentContext(ctx);

		if (ctx) {
			ctx->SetDrawSurface(draw);
			ctx->SetReadSurface(read);
		}

		return EGL_TRUE;

	}


	GLAPI EGLContext APIENTRY eglGetCurrentContext (void) {
		return Context::GetCurrentContext();
	}


	GLAPI EGLSurface APIENTRY eglGetCurrentSurface (EGLint readdraw) {
		EGLContext currentContext = eglGetCurrentContext();

		if (currentContext != 0) {
			switch (readdraw) {
			case EGL_DRAW:
				return currentContext->GetDrawSurface();

			case EGL_READ:
				return currentContext->GetReadSurface();

			default:
				return 0;
			}
		} else {
			return 0;
		}
	}


	GLAPI EGLDisplay APIENTRY eglGetCurrentDisplay (void) {
		return 0;
	}


	GLAPI EGLBoolean APIENTRY eglQueryContext (EGLDisplay dpy, EGLContext ctx, EGLint attribute, EGLint *value) {
		*value = ctx->GetConfig()->GetConfigAttrib(attribute);
		return EGL_TRUE;
	}


	GLAPI EGLBoolean APIENTRY eglWaitGL (void) {
		return EGL_TRUE;
	}


	GLAPI EGLBoolean APIENTRY eglWaitNative (EGLint engine) {
		return EGL_TRUE;
	}


	GLAPI EGLBoolean APIENTRY eglSwapBuffers (EGLDisplay dpy, EGLSurface draw) {

		Context::GetCurrentContext()->Flush();
		NativeDisplayType nativeDisplay = reinterpret_cast<NativeDisplayType>(dpy);

		nativeDisplay->BitBlt(TPoint(0, 0), draw->GetBitmap());

		return EGL_TRUE;
	}

	GLAPI EGLBoolean APIENTRY eglCopyBuffers (EGLDisplay dpy, EGLSurface surface, NativePixmapType target) {
		if (!target) {
			return EGL_BAD_NATIVE_PIXMAP;
		}

		Context::GetCurrentContext()->Flush();

	#if 0
		HDC nativeDisplay = GetNativeDisplay(dpy);
		HDC memoryDC = surface->GetMemoryDC();
		HDC targetDC = CreateCompatibleDC(nativeDisplay);;

		if (memoryDC == INVALID_HANDLE_VALUE) {
			memoryDC = CreateCompatibleDC(nativeDisplay);
		}

		SelectObject(memoryDC, surface->GetBitmap());
		SelectObject(targetDC, target);

		if (!BitBlt(targetDC, 0, 0, surface->GetWidth(), surface->GetHeight(), memoryDC, 0, 0, SRCCOPY))
			return EGL_BAD_MATCH;

		if (memoryDC != surface->GetMemoryDC()) {
			DeleteDC(memoryDC);
		}

		DeleteDC(targetDC);
	#endif
		return EGL_TRUE;
	}

}
