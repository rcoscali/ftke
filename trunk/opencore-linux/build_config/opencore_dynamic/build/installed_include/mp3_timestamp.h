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
#ifndef MP3_TIMESTAMP_H_INCLUDED
#define MP3_TIMESTAMP_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OMX_Component_h
#include "OMX_Component.h"
#endif



#define DEFAULT_SAMPLING_FREQ_MP3 44100
#define DEFAULT_SAMPLES_PER_FRAME_MP3 1152

class Mp3TimeStampCalc
{
    public:

        Mp3TimeStampCalc()
        {
            iSamplingFreq = DEFAULT_SAMPLING_FREQ_MP3;
            iCurrentTs = 0;
            iCurrentSamples = 0;
            iSamplesPerFrame = DEFAULT_SAMPLES_PER_FRAME_MP3;
        };

        void SetParameters(uint32 aFreq, uint32 aSamples);

        void SetFromInputTimestamp(OMX_TICKS aValue);

        void UpdateTimestamp(uint32 aValue);

        OMX_TICKS GetConvertedTs();

        OMX_TICKS GetCurrentTimestamp();

        OMX_TICKS GetFrameDuration();


    private:
        uint32 iSamplingFreq;
        OMX_TICKS iCurrentTs;
        uint32 iCurrentSamples;
        uint32 iSamplesPerFrame;
};

#endif  //#ifndef MP3_TIMESTAMP_H_INCLUDED
