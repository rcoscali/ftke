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
#ifndef TEST_PV_AUTHOR_ENGINE_TESTSET6_H_INCLUDED
#define TEST_PV_AUTHOR_ENGINE_TESTSET6_H_INCLUDED

#ifndef TEST_PV_MEDIAINPUT_AUTHOR_ENGINE_H
#include "test_pv_mediainput_author_engine.h"
#endif

#define PVPATB_TEST_IS_TRUE( condition ) (iTestCase->test_is_true_stub( (condition), (#condition), __FILE__, __LINE__ ))

class pv_mediainput_async_test_errorhandling: public pvauthor_async_test_base
{
    public:
        pv_mediainput_async_test_errorhandling(PVAuthorAsyncTestParam aTestParam, PVMediaInputTestParam aMediaParam, bool aPauseResumeEnable, uint32 aTestErrorType)
                : pvauthor_async_test_base(aTestParam)
                , iOutputFileName(NULL)
                , iInputFileName(NULL)
                , iComposerMimeType(aMediaParam.iComposerInfo)
                , iAudioEncoderMimeType(aMediaParam.iAudioEncInfo)
                , iVideoEncoderMimeType(aMediaParam.iVideoEncInfo)
                , iMediaInputType(aMediaParam.iInputFormat)
                , iAuthor(NULL)
                , iComposer(NULL)
                , iFileParser(NULL)
                , iComposerConfig(NULL)
                , iAudioEncoderConfig(NULL)
                , iVideoEncoderConfig(NULL)
                , iPendingCmds(0)
                , iPauseResumeEnable(aPauseResumeEnable)
                , iRemoveDataSourceDone(0)
                , iAddAudioMediaTrack(false)
                , iAddVideoMediaTrack(false)
                , iTestErrorType(aTestErrorType)

        {
            iLogger = PVLogger::GetLoggerObject("pv_mediainput_async_test_errorhandling");

            iTestDuration = KTestDuration * 1000 * 1000;

            if (oscl_strlen(aMediaParam.iIPFileInfo.get_cstr()) != 0)
            {
                oscl_wchar output2[ARRAY_SIZE];
                oscl_UTF8ToUnicode(aMediaParam.iIPFileInfo.get_cstr(), oscl_strlen(aMediaParam.iIPFileInfo.get_cstr()), output2, ARRAY_SIZE);
                iInputFileName.set(output2, oscl_strlen(output2));

            }

            if (oscl_strlen(aMediaParam.iOPFileInfo.get_cstr()) != 0)
            {
                oscl_wchar output1[ARRAY_SIZE];
                oscl_UTF8ToUnicode(aMediaParam.iOPFileInfo.get_cstr(), oscl_strlen(aMediaParam.iOPFileInfo.get_cstr()), output1, ARRAY_SIZE);
                iOutputFileName.set(output1, oscl_strlen(output1));
            }

        }

        ~pv_mediainput_async_test_errorhandling()
        {
            Cleanup();
        }

        void Run();
        void StartTest();

        // Author engine observer functions
        void HandleErrorEvent(const PVAsyncErrorEvent& aEvent);
        void HandleInformationalEvent(const PVAsyncInformationalEvent& aEvent);
        void CommandCompleted(const PVCmdResponse& aResponse);
        void ConfigMp4Composer();
        PVAECmdType iState;
        // Test output
        OSCL_wHeapString<OsclMemAllocator> iOutputFileName;
        OSCL_wHeapString<OsclMemAllocator> iInputFileName;

        OSCL_HeapString<OsclMemAllocator> iComposerMimeType;
        OSCL_HeapString<OsclMemAllocator> iAudioEncoderMimeType;
        OSCL_HeapString<OsclMemAllocator> iVideoEncoderMimeType;

        //test input type
        PVMFFormatType  iMediaInputType;

        typedef struct ts_keytype
        {
            uint8 mode;
            uint32 duration;
            uint32 track_no;
        } key_type;

    private:
        // Methods to create test input nodes and add to author engine
        bool CreateTestInputs();
        bool AddDataSource();

        // Methods to configure composers
        bool ConfigComposer();
        bool ConfigOutputFile();
        bool ConfigMp43gpComposer();

        // Methods to add media tracks
        bool AddMediaTrack();

        // Methods to configure encoders
        bool ConfigureVideoEncoder();
        bool ConfigureAudioEncoder();

        void ResetAuthorConfig();

        void Cleanup();
        void RemoveMIOComp();
        bool CapConfigSync1();

        void AddEngineCommand()
        {
            iPendingCmds++;
        }

        bool EngineCmdComplete()
        {
            if (iPendingCmds > 0)
            {
                --iPendingCmds;
            }

            if (iPendingCmds == 0)
            {
                return true;
            }

            return false;
        }

        bool IsEngineCmdPending()
        {
            if (iPendingCmds > 0)
                return true;
            else
                return false;
        }

        // Author engine related variables
        PVAuthorEngineInterface* iAuthor;
        OsclAny*                 iComposer;
        OsclAny*                 iFileParser;
        PVInterface*             iComposerConfig;
        PVInterface*             iAudioEncoderConfig;
        PVInterface*             iVideoEncoderConfig;
        PVMIOControlComp         iMIOComponent;
        uint32                   iPendingCmds;
        PVLogger*                iLogger;
        bool                     iPauseResumeEnable;
        uint                     iRemoveDataSourceDone;
        uint32                   iTestDuration;
        bool                     iAddAudioMediaTrack;
        bool                     iAddVideoMediaTrack;
        Oscl_FileServer          iFileServer;
        uint32                   iTestErrorType;
        PvmiCapabilityAndConfig* iAuthorCapConfigIF;
        PvmiCapabilityAndConfig* iAuthorCapConfigIF1;
        PvmiMIOControl*          iMediaIOControl;
        PvmiKvp* iErrorKVP;
        FILE*                    iFile;
        PVInterface*             iOutputSizeAndDurationConfig;
        PVAECmdType              iCheckState;
};

#endif
