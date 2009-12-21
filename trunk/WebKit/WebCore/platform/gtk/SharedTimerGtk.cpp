/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2006 Michael Emmel mike.emmel@gmail.com
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
#include "SharedTimer.h"

#include <wtf/Assertions.h>
#include <wtf/CurrentTime.h>
#include <glib.h>

namespace WebCore {

static FtkSource* source = NULL;
static void (*sharedTimerFiredFunction)();

void setSharedTimerFiredFunction(void (*f)())
{
    sharedTimerFiredFunction = f;
}

static Ret timeout_cb(void* ctx)
{
    if (sharedTimerFiredFunction)
        sharedTimerFiredFunction();
    return FALSE;
}

void setSharedTimerFireTime(double fireTime)
{
    ASSERT(sharedTimerFiredFunction);
    double interval = fireTime - currentTime();
    guint intervalInMS;
    if (interval < 0)
        intervalInMS = 0;
    else {
        intervalInMS = (guint)interval;
    }

    stopSharedTimer();
    if (intervalInMS == 0)
    	source = ftk_source_idle_create(timeout_cb, NULL);
    else
    	source = ftk_source_timer_create(intervalInMS, timeout_cb, NULL);
    ftk_main_loop_add_source(ftk_default_main_loop(), source);
}

void stopSharedTimer()
{
    gboolean s = FALSE;
    if (source == 0)
        return;

    ftk_main_loop_remove_source(ftk_default_main_loop(), source);
    source = 0;
}

}
