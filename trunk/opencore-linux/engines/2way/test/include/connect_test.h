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
#ifndef CONNECT_TEST_H_INCLUDED
#define CONNECT_TEST_H_INCLUDED

#include "test_base.h"


class connect_test : public test_base
{
    public:
        connect_test(bool aUseProxy, int aMaxRuns, bool runTimerTest = false) :
                test_base(PVMF_MIME_AMR_IF2, PVMF_MIME_AMR_IF2, PVMF_MIME_YUV420, PVMF_MIME_YUV420, aUseProxy, aMaxRuns) ,
                iRunTimerTest(runTimerTest),
                i324mConfigInterface(NULL),
                iMP4H263EncoderInterface(NULL) {};

        ~connect_test()
        {
        }

        void test();

        void Run();

        void DoCancel();

        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);


    private:
        virtual void InitSucceeded();
        virtual void InitFailed();
        virtual void EncoderIFSucceeded();
        virtual void EncoderIFFailed();
        virtual void ConnectFailed();
        virtual void ConnectCancelled();
        virtual void DisCmdSucceeded();
        virtual void DisCmdFailed();

        bool iRunTimerTest;
        PVInterface *i324mConfigInterface;
        PVInterface *iMP4H263EncoderInterface;
};


#endif


