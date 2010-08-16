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
#ifndef PV_2WAY_CMD_CONTROL_DATAPATH_H_INCLUDED
#define PV_2WAY_CMD_CONTROL_DATAPATH_H_INCLUDED

#ifndef PV_2WAY_DATAPATH_H_INCLUDED
#include "pv_2way_datapath.h"
#endif

class CPV2WayCmdControlDatapath : public CPV2WayDatapath
{
    public:
        CPV2WayCmdControlDatapath(PVLogger *aLogger,
                                  TPV2WayDatapathType aType,
                                  PVMFFormatType aFormat,
                                  CPV324m2Way *a2Way) : CPV2WayDatapath(aLogger, aType, aFormat, a2Way),
                iCmdInfo(NULL)

        {};

        virtual ~CPV2WayCmdControlDatapath() {};

        TPV2WayCmdInfo *GetCmdInfo()
        {
            return iCmdInfo;
        }
        virtual bool SetCmd(TPV2WayCmdInfo *aCmdInfo) = 0;

    protected:
        bool CloseDatapath(TPV2WayCmdInfo *aCmdInfo);

        void CommandComplete(PVMFStatus aStatus);

        TPV2WayCmdInfo *iCmdInfo;
};


#endif //PV_2WAY_CMD_CONTROL_DATAPATH_H_INCLUDED


