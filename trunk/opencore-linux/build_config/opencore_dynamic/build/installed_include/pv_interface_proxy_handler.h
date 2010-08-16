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


#ifndef PV_INTERFACE_PROXY_HANDLER_H_INCLUDED
#define PV_INTERFACE_PROXY_HANDLER_H_INCLUDED

#ifndef PV_INTERFACE_PROXY_H_INCLUDED
#include "pv_interface_proxy.h"
#endif

#include "oscl_scheduler_ao.h"

class CPVInterfaceProxyHandler :
        public OsclActiveObject
{
    public:
        CPVInterfaceProxyHandler(CPVInterfaceProxy *aProxy, int32 pri):
                OsclActiveObject(pri, "proxyHandler")
                , iProxy(aProxy)
        {}

        virtual ~CPVInterfaceProxyHandler()
        {}

    private:
        void Run();

        CPVInterfaceProxy *iProxy;
};

#endif //


