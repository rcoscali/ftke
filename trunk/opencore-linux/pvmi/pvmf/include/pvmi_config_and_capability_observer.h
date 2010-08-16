/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
#ifndef PVMI_CONFIG_AND_CAPABILITY_OBSERVER_H_INCLUDED
#define PVMI_CONFIG_AND_CAPABILITY_OBSERVER_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

/**
 * This class defines an abstract interface for capability and configuration observer
 */
class PvmiConfigAndCapabilityCmdObserver
{
    public:
        /**
         * This method allows to send a signal of completion for a particular event
         */
        virtual void SignalEvent(int32 req_id) = 0;
        virtual ~PvmiConfigAndCapabilityCmdObserver() {}
};

#endif //PVMI_CONFIG_AND_CAPABILITY_OBSERVER_H_INCLUDED
