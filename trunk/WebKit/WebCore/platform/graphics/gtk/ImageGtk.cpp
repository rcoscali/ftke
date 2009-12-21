/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#include "BitmapImage.h"
#include "CString.h"
#include "GOwnPtr.h"

#include <cairo.h>
#include <ftk.h>
#include <glib.h>

namespace WebCore {

static PassRefPtr<SharedBuffer> loadResourceSharedBuffer(const char* name)
{
    CString fileName;

    // Find the path for the image
    fileName = String::format("%s/webkit-1.0/images/%s.png", DATA_DIR, name).utf8();

    GOwnPtr<gchar> content;
    gsize length;
    if (!g_file_get_contents(fileName.data(), &content.outPtr(), &length, 0))
        return SharedBuffer::create();

    return SharedBuffer::create(content.get(), length);
}

void BitmapImage::initPlatformData()
{
}

void BitmapImage::invalidatePlatformData()
{
}

PassRefPtr<Image> Image::loadPlatformResource(const char* name)
{
    RefPtr<BitmapImage> img = BitmapImage::create();
    RefPtr<SharedBuffer> buffer = loadResourceSharedBuffer(name);
    img->setData(buffer.release(), true);
    return img.release();
}

static inline unsigned char* getCairoSurfacePixel(unsigned char* data, uint x, uint y, uint rowStride)
{
    return data + (y * rowStride) + x * 4;
}

static inline guchar* get_pixel(guchar* data, uint x, uint y, uint rowStride)
{
    return data + (y * rowStride) + x * 4;
}

FtkBitmap* BitmapImage::getBitmap()
{
	FtkColor bg; 
    int width = cairo_image_surface_get_width(frameAtIndex(currentFrame()));
    int height = cairo_image_surface_get_height(frameAtIndex(currentFrame()));
    unsigned char* surfaceData = cairo_image_surface_get_data(frameAtIndex(currentFrame()));
    int surfaceRowStride = cairo_image_surface_get_stride(frameAtIndex(currentFrame()));

	bg.a = 0xff;
	FtkBitmap* dest = ftk_bitmap_create(width, height, bg);

    if (!dest)
        return 0;

    guchar* pixbufData = (guchar*)ftk_bitmap_bits(dest);
    int pixbufRowStride = width * 4;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char* source = getCairoSurfacePixel(surfaceData, x, y, surfaceRowStride);
            guchar* dest = get_pixel(pixbufData, x, y, pixbufRowStride);

#if G_BYTE_ORDER == G_LITTLE_ENDIAN
            guchar alpha = source[3];
            dest[0] = alpha ? ((source[2] * 255) / alpha) : 0;
            dest[1] = alpha ? ((source[1] * 255) / alpha) : 0;
            dest[2] = alpha ? ((source[0] * 255) / alpha) : 0;
            dest[3] = alpha;
#else
            guchar alpha = source[0];
            dest[0] = alpha ? ((source[1] * 255) / alpha) : 0;
            dest[1] = alpha ? ((source[2] * 255) / alpha) : 0;
            dest[2] = alpha ? ((source[3] * 255) / alpha) : 0;
            dest[3] = alpha;
#endif
        }
    }

    return dest;
}

}
