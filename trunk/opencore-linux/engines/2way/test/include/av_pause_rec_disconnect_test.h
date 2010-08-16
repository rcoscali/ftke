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
#ifndef AV_PAUSE_REC_DISCONNECT_TEST_H_INCLUDED
#define AV_PAUSE_REC_DISCONNECT_TEST_H_INCLUDED

#include "test_base.h"


class av_pause_rec_disconnect_test : public test_base
{
    public:
        av_pause_rec_disconnect_test(bool aUseProxy) : test_base(aUseProxy),
                iAudioRecPaused(false),
                iAudioStartRecId(0),
                iAudioPauseRecId(0),
                iVideoRecPaused(false),
                iVideoStartRecId(0),
                iVideoPauseRecId(0)
        {};

        ~av_pause_rec_disconnect_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent);

        void CommandCompletedL(const CPVCmnCmdResp& aResponse);


    private:
        bool start_async_test();

        bool check_rec_paused()
        {
            return (iAudioRecPaused && iVideoRecPaused);
        }

        bool iAudioRecPaused;
        TPVCmnCommandId iAudioStartRecId;
        TPVCmnCommandId iAudioPauseRecId;
        bool iVideoRecPaused;
        TPVCmnCommandId iVideoStartRecId;
        TPVCmnCommandId iVideoPauseRecId;
};


#endif


