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
#include "audio_only_pause_test.h"



void audio_only_pause_test::test()
{
    fprintf(fileoutput, "Start audio only pause test, num runs %d, proxy %d.\n", iMaxRuns, iUseProxy);
    int error = 0;

    scheduler = OsclExecScheduler::Current();

    this->AddToScheduler();

    if (start_async_test())
    {
        OSCL_TRY(error, scheduler->StartScheduler());
        if (error != 0)
        {
            OSCL_LEAVE(error);
        }
    }

    destroy_sink_source();

    this->RemoveFromScheduler();
}


void audio_only_pause_test::Run()
{
    if (terminal)
    {
        if (iUseProxy)
        {
            CPV2WayProxyFactory::DeleteTerminal(terminal);
        }
        else
        {
            CPV2WayEngineFactory::DeleteTerminal(terminal);
        }
        terminal = NULL;
    }

    scheduler->StopScheduler();
}

void audio_only_pause_test::DoCancel()
{
}


void audio_only_pause_test::HandleInformationalEventL(const CPVCmnAsyncInfoEvent& aEvent)
{
    int error = 0;

    switch (aEvent.GetEventType())
    {
        case PVT_INDICATION_INCOMING_TRACK:
            TPVChannelId id;

            if (((CPVCmnAsyncEvent&)aEvent).GetLocalBuffer()[0] == PV_AUDIO)
            {
                OSCL_TRY(error, iAudioAddSinkId = terminal->AddDataSinkL(*iAudioSink, id));
                if (error)
                {
                    test_is_true(false);
                    disconnect();
                }
            }
            break;

        case PVT_INDICATION_DISCONNECT:
            iAudioSourceAdded = false;
            iVideoSourceAdded = false;
            iAudioSinkAdded = false;
            iVideoSinkAdded = false;
            break;

        case PVT_INDICATION_CLOSE_TRACK:
            if (((CPVCmnAsyncEvent&)aEvent).GetLocalBuffer()[0] == PV_AUDIO)
            {
                if (((CPVCmnAsyncEvent&)aEvent).GetLocalBuffer()[1] == INCOMING)
                {
                    iAudioSinkAdded = false;
                }
                else
                {
                    iAudioSourceAdded = false;
                }
            }
            break;

        case PVT_INDICATION_INTERNAL_ERROR:
            break;

        default:
            break;
    }
}

void audio_only_pause_test::CommandCompletedL(const CPVCmnCmdResp& aResponse)
{
    int error = 0;
    switch (aResponse.GetCmdType())
    {
        case PVT_COMMAND_INIT:
            if (aResponse.GetCmdStatus() == PVMFSuccess)
            {
                OSCL_TRY(error, terminal->ConnectL(iConnectOptions));
                if (error)
                {
                    test_is_true(false);
                    reset();
                }
            }
            else
            {
                test_is_true(false);
                RunIfNotReady();
            }
            break;

        case PVT_COMMAND_RESET:
            RunIfNotReady();
            break;

        case PVT_COMMAND_ADD_DATA_SOURCE:
            if (aResponse.GetCmdId() == iAudioAddSourceId)
            {
                if (aResponse.GetCmdStatus() == PVMFSuccess)
                {
                    iAudioSourceAdded = true;

                    OSCL_TRY(error, iAudioPauseSourceId = terminal->PauseL(*iAudioSource));
                    if (error)
                    {
                        test_is_true(false);
                        disconnect();
                    }
                }
                else
                {
                    test_is_true(false);
                    disconnect();
                }
                iAudioAddSourceId = 0;
            }
            break;

        case PVT_COMMAND_REMOVE_DATA_SOURCE:
            if (aResponse.GetCmdId() == iAudioRemoveSourceId)
            {
                iAudioRemoveSourceId = 0;
                iAudioSourceAdded = false;
            }
            break;

        case PVT_COMMAND_ADD_DATA_SINK:
            if (aResponse.GetCmdId() == iAudioAddSinkId)
            {
                if (aResponse.GetCmdStatus() == PVMFSuccess)
                {
                    iAudioSinkAdded = true;

                    OSCL_TRY(error, iAudioPauseSinkId = terminal->PauseL(*iAudioSink));
                    if (error)
                    {
                        test_is_true(false);
                        disconnect();
                    }
                }
                else
                {
                    test_is_true(false);
                    disconnect();
                }
                iAudioAddSinkId = 0;
            }
            break;

        case PVT_COMMAND_REMOVE_DATA_SINK:
            if (aResponse.GetCmdId() == iAudioRemoveSinkId)
            {
                iAudioRemoveSinkId = 0;
                iAudioSinkAdded = false;
            }
            break;

        case PVT_COMMAND_CONNECT:
            if (aResponse.GetCmdStatus() == PVMFSuccess)
            {
                OSCL_TRY(error, iAudioAddSourceId = terminal->AddDataSourceL(*iAudioSource));
                if (error)
                {
                    test_is_true(false);
                    disconnect();
                }
            }
            else
            {
                test_is_true(false);
                reset();
            }
            break;

        case PVT_COMMAND_DISCONNECT:
            iAudioSourceAdded = false;
            iVideoSourceAdded = false;
            iAudioSinkAdded = false;
            iVideoSinkAdded = false;
            reset();
            break;

        case PVT_COMMAND_PAUSE:
            if (aResponse.GetCmdStatus() == PVMFSuccess)
            {
                if (aResponse.GetCmdId() == iAudioPauseSourceId)
                {
                    OSCL_TRY(error, iAudioResumeSourceId = terminal->ResumeL(*iAudioSource));
                }
                else if (aResponse.GetCmdId() == iAudioPauseSinkId)
                {
                    OSCL_TRY(error, iAudioResumeSinkId = terminal->ResumeL(*iAudioSink));
                }
                else
                {
                    error = -1;
                }

                if (error)
                {
                    test_is_true(false);
                    disconnect();
                }
            }
            else
            {
                test_is_true(false);
                disconnect();
            }
            break;

        case PVT_COMMAND_RESUME:
            if (aResponse.GetCmdStatus() == PVMFSuccess)
            {
                if (aResponse.GetCmdId() == iAudioResumeSourceId)
                {
                    iAudioSourceResumed = true;
                }
                else if (aResponse.GetCmdId() == iAudioResumeSinkId)
                {
                    iAudioSinkResumed = true;
                }
                else
                {
                    test_is_true(false);
                    disconnect();
                    break;
                }

                if (check_audio_resumed())
                {
                    iCurrentRun++;

                    if (iCurrentRun < iMaxRuns)
                    {
                        OSCL_TRY(error, iAudioPauseSourceId = terminal->PauseL(*iAudioSource));
                        if (error)
                        {
                            test_is_true(false);
                            disconnect();
                        }
                        else
                        {
                            OSCL_TRY(error, iAudioPauseSinkId = terminal->PauseL(*iAudioSink));
                            if (error)
                            {
                                test_is_true(false);
                                disconnect();
                            }
                        }
                        iAudioSourceResumed = false;
                        iAudioSinkResumed = false;
                    }
                    else
                    {
                        test_is_true(true);
                        disconnect();
                    }
                }
            }
            else
            {
                test_is_true(false);
                disconnect();
            }
            break;

        case PVT_COMMAND_CANCEL_ALL_COMMANDS:
            break;
    }
}

bool audio_only_pause_test::start_async_test()
{
    int error = 0;

    if (iUseProxy)
    {
        OSCL_TRY(error, terminal = CPV2WayProxyFactory::CreateTerminalL(PV_324M,
                                   (MPVCmnCmdStatusObserver *) this,
                                   (MPVCmnInfoEventObserver *) this,
                                   (MPVCmnErrorEventObserver *) this));
    }
    else
    {
        OSCL_TRY(error, terminal = CPV2WayEngineFactory::CreateTerminalL(PV_324M,
                                   (MPVCmnCmdStatusObserver *) this,
                                   (MPVCmnInfoEventObserver *) this,
                                   (MPVCmnErrorEventObserver *) this));
    }

    if (error)
    {
        test_is_true(false);
        return false;
    }

    create_sink_source();

    OSCL_TRY(error, terminal->InitL(iSdkInitInfo, iCommServer));
    if (error)
    {
        test_is_true(false);
        if (iUseProxy)
        {
            CPV2WayProxyFactory::DeleteTerminal(terminal);
        }
        else
        {
            CPV2WayEngineFactory::DeleteTerminal(terminal);
        }
        terminal = NULL;
        return false;
    }

    return true;
}





