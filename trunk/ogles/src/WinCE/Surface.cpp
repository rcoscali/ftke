// ==========================================================================
//
// Surface.cpp		Drawing Surface Class for 3D Rendering Library
//
// --------------------------------------------------------------------------
//
// 08-14-2003		Hans-Martin Will	initial version
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
#include "Surface.h"
#include "Color.h"
#include <string.h>


using namespace EGL;


namespace {
	struct InfoHeader {
		BITMAPINFOHEADER bmiHeader;
		DWORD            bmiColors[3];

		InfoHeader(U32 width, U32 height) {
			bmiHeader.biSize = sizeof(bmiHeader);
			bmiHeader.biWidth = width;
			bmiHeader.biHeight = height;
			bmiHeader.biPlanes = 1;
			bmiHeader.biBitCount = 16;
			bmiHeader.biCompression = BI_BITFIELDS;
			bmiHeader.biSizeImage = width * height * sizeof(U16);
			bmiHeader.biXPelsPerMeter = 72 * 25;
			bmiHeader.biYPelsPerMeter = 72 * 25;
			bmiHeader.biClrUsed = 0;
			bmiHeader.biClrImportant = 0;

			bmiColors[0] = 0xF800;
			bmiColors[1] = 0x07E0;
			bmiColors[2] = 0x001F;
		}
	};
}


Surface :: Surface(const Config & config, HDC hdc)
:	m_Config(config),
	m_Rect (0, 0, config.GetConfigAttrib(EGL_WIDTH), config.GetConfigAttrib(EGL_HEIGHT)),
	m_Bitmap(reinterpret_cast<HBITMAP>(INVALID_HANDLE_VALUE)),
	m_HDC(reinterpret_cast<HDC>(INVALID_HANDLE_VALUE))
{
	//m_ColorBuffer = new U16[m_Width * m_Height];
	U32 width = GetWidth();
	U32 height = GetHeight();

	m_AlphaBuffer = new U8[width * height];
	m_DepthBuffer = new U16[width * height];
	m_StencilBuffer = new U32[width * height];

	if (hdc != INVALID_HANDLE_VALUE) {
		m_HDC = CreateCompatibleDC(hdc);
	}

	InfoHeader info(width, height);;

	m_Bitmap = CreateDIBSection(m_HDC, reinterpret_cast<BITMAPINFO *>(&info), DIB_RGB_COLORS,
		reinterpret_cast<void **>(&m_ColorBuffer), NULL, 0);
}


Surface :: ~Surface() {


	if (m_Bitmap != INVALID_HANDLE_VALUE) {
		DeleteObject(m_Bitmap);
		m_Bitmap = reinterpret_cast<HBITMAP>(INVALID_HANDLE_VALUE);
	}

	if (m_HDC != INVALID_HANDLE_VALUE) {
		DeleteDC(m_HDC);
		m_HDC = reinterpret_cast<HDC>(INVALID_HANDLE_VALUE);
	}

	/*if (m_ColorBuffer != 0) {
		delete [] m_ColorBuffer;
		m_ColorBuffer = 0;
	}*/

	if (m_AlphaBuffer != 0) {
		delete [] m_AlphaBuffer;
		m_AlphaBuffer = 0;
	}

	if (m_DepthBuffer != 0) {
		delete[] m_DepthBuffer;
		m_DepthBuffer = 0;
	}

	if (m_StencilBuffer != 0) {
		delete[] m_StencilBuffer;
		m_StencilBuffer = 0;
	}
}


void Surface :: Dispose() {
	if (GetCurrentContext() != 0) {
		m_Disposed = true;
	} else {
		delete this;
	}
}


void Surface :: SetCurrentContext(Context * context) {
	m_CurrentContext = context;

	if (context == 0 && m_Disposed) {
		delete this;
	}
}


namespace {

	template <class T> void FillRect(T * base, const Rect & bufferRect, const Rect & fillRect,
		const T& value, const T& mask) {
		Rect rect = Rect::Intersect(fillRect, bufferRect);

		base += fillRect.x + fillRect.y * bufferRect.width;
		size_t gap = bufferRect.width - fillRect.width;

		size_t rows = fillRect.height;
		T inverseMask = ~mask;
		T maskedValue = value & mask;

		while (rows--) {
			for (size_t columns = fillRect.width; columns > 0; columns--) {
				*base = (*base & inverseMask) | maskedValue;
				++base;
			}

			base += gap;
		}
	}

	template <class T> void FillRect(T * base, const Rect & bufferRect, const Rect & fillRect,
		const T& value) {
		Rect rect = Rect::Intersect(fillRect, bufferRect);

		base += fillRect.x + fillRect.y * bufferRect.width;
		size_t gap = bufferRect.width - fillRect.width;

		size_t rows = fillRect.height;

		while (rows--) {
			for (size_t columns = fillRect.width; columns > 0; columns--) {
				*base = value;
				++base;
			}

			base += gap;
		}
	}
}


void Surface :: ClearDepthBuffer(U16 depth, bool mask, const Rect& scissor) {

	if (!mask || !m_DepthBuffer)
		return;

	FillRect(m_DepthBuffer, GetRect(), scissor, depth);
}


void Surface :: ClearStencilBuffer(U32 value, U32 mask, const Rect& scissor) {

	if (!mask || !m_StencilBuffer)
		return;

	if (mask != ~0) {
		FillRect(m_StencilBuffer, GetRect(), scissor, value, mask);
	} else {
		FillRect(m_StencilBuffer, GetRect(), scissor, value);
	}
}

/*
I32 Surface :: DepthBitsFromDepth(GLclampx depth) {
	I32 result;
	gppMul_16_32s(EGL_CLAMP(depth, EGL_FIXED_0, EGL_FIXED_1), 0xffffff, &result);
	return result;
}
*/

void Surface :: ClearColorBuffer(const Color & rgba, const Color & mask, const Rect& scissor) {
	U16 color = rgba.ConvertTo565();
	U16 colorMask = mask.ConvertTo565();

	if (colorMask == 0xffff) {
		FillRect(m_ColorBuffer, GetRect(), scissor, color);
	} else {
		FillRect(m_ColorBuffer, GetRect(), scissor, color, colorMask);
	}

	if (mask.A() && m_AlphaBuffer)
		FillRect(m_AlphaBuffer, GetRect(), scissor, rgba.A());

}


bool Surface :: Save(const TCHAR * filename) {

	InfoHeader info(GetWidth(), GetHeight());

    BITMAPFILEHEADER header;
    header.bfType      = 0x4d42;
    header.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(info) + info.bmiHeader.biSizeImage;
    header.bfReserved1 = 0;
    header.bfReserved2 = 0;
    header.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(info);

    // Create the file
    HANDLE hFile = ::CreateFile(filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

    if (hFile == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD temp;

    // Write the header + bitmap info
    ::WriteFile(hFile, &header, sizeof(header), &temp, 0);
    ::WriteFile(hFile, &info, sizeof(info), &temp, 0);

    // Write the image
    U16 * pixels = m_ColorBuffer;

    for (int h = GetHeight(); h; --h) {
        ::WriteFile(hFile, pixels, GetWidth() * sizeof(U16), &temp, 0 );
        pixels += GetWidth();
    }

    ::CloseHandle(hFile);

    return true;
}

