/*
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
 * Copyright (C) 2007, 2009 Holger Hans Peter Freyther
 * All rights reserved.
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
#include "Widget.h"

#include "Cursor.h"
#include "FrameView.h"
#include "GraphicsContext.h"
#include "HostWindow.h"
#include "IntRect.h"
#include "RenderObject.h"

#include <ftk.h>

namespace WebCore {

static GdkCursor* lastSetCursor;

Widget::Widget(PlatformWidget widget)
{
    init(widget);
}

Widget::~Widget()
{
    ASSERT(!parent());
    releasePlatformWidget();
}

void Widget::setFocus()
{
    ftk_widget_set_focused(platformWidget() ? platformWidget() : (FtkWidget*)(root()->hostWindow()->platformPageClient()));
}

    
void Widget::setCursor(const Cursor& cursor)
{
	/*TODO:*/
}

void Widget::show()
{
    if (!platformWidget())
         return;
    ftk_widget_show(platformWidget(), 1);
}

void Widget::hide()
{
    if (!platformWidget())
         return;
    ftk_widget_show(platformWidget(), 0);
}

void Widget::paint(GraphicsContext* context, const IntRect& rect)
{
	
}

void Widget::setIsSelected(bool isSelected)
{
    if (!platformWidget())
        return;

	/*TODO*/
}

IntRect Widget::frameRect() const
{
    return m_frame;
}

void Widget::setFrameRect(const IntRect& rect)
{
    m_frame = rect;
}

void Widget::releasePlatformWidget()
{
    if (!platformWidget())
         return;
    ftk_widget_unref(platformWidget());
}

void Widget::retainPlatformWidget()
{
    if (!platformWidget())
         return;
    ftk_widget_ref(platformWidget());
}

}
