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
/**
 *
 * @file pvmf_fileoutput_inport.h
 * @brief Input port for simple file output node
 *
 */

#include "pvmf_dummy_fileoutput_inport.h"
#include "oscl_error_codes.h"
#include "pv_mime_string_utils.h"
#include "pvmf_dummy_fileoutput_node.h"
#include "pvmf_media_cmd.h"
#include "pvmf_media_msg_format_ids.h"

#define LOG_STACK_TRACE(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, m);
#define LOG_DEBUG(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_DEBUG, m);
#define LOG_ERR(m) PVLOGGER_LOGMSG(PVLOGMSG_INST_REL,iLogger,PVLOGMSG_ERR,m);

////////////////////////////////////////////////////////////////////////////
PVMFDummyFileOutputInPort::~PVMFDummyFileOutputInPort()
{
    Disconnect();
    ClearMsgQueues();
    iDataQueue.Clear();
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::Start()
{
    iState = PORT_STATE_STARTED;
    RunIfNotReady();
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::Pause()
{
    iState = PORT_STATE_BUFFERING;
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::Stop()
{
    iDataQueue.Clear();
    iState = PORT_STATE_BUFFERING;
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::addRef()
{
    ++iExtensionRefCount;
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::removeRef()
{
    if (iExtensionRefCount > 0)
    {
        --iExtensionRefCount;
    }
}

////////////////////////////////////////////////////////////////////////////
bool PVMFDummyFileOutputInPort::queryInterface(const PVUuid& uuid, PVInterface*& iface)
{
    if (uuid == PvmfNodesSyncControlUuid)
    {
        PvmfNodesSyncControlInterface* myInterface = OSCL_STATIC_CAST(PvmfNodesSyncControlInterface*, this);
        iface = OSCL_STATIC_CAST(PVInterface*, myInterface);
        addRef();
    }
    else
    {
        iface = NULL;
        return false;
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PVMFDummyFileOutputInPort::SetClock(PVMFMediaClock* aClock)
{
    return iDataQueue.SetClock(aClock);
}

/////////////////////////////////////////////////////////////////////////////
PVMFStatus PVMFDummyFileOutputInPort::ChangeClockRate(int32 aRate)
{
    OSCL_UNUSED_ARG(aRate);
    return PVMFSuccess;
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PVMFDummyFileOutputInPort::SetMargins(int32 aEarlyMargin, int32 aLateMargin)
{
    return iSyncUtil.SetMargins(aEarlyMargin, aLateMargin);
}

////////////////////////////////////////////////////////////////////////////
PVMFCommandId PVMFDummyFileOutputInPort::SkipMediaData(PVMFSessionId aSessionId,
        PVMFTimestamp aResumeTimestamp,
        uint32 aStreamID,
        bool aPlayBackPositionContinuous,
        OsclAny* aContext)
{
    OSCL_UNUSED_ARG(aStreamID);
    OSCL_UNUSED_ARG(aPlayBackPositionContinuous);

    // To handle case where media data are in queues but
    // not processed yet
    if (iLastDataTimestampSet == false)
    {
        // Check if there is data in the incoming queue
        if (IncomingMsgQueueSize() > 0)
        {
            iLastDataTimestamp = iIncomingQueue.iQ.front()->getTimestamp();
            if (iLastDataTimestamp == PVMF_MAX_CMDID_CNTR)
            {
                // TEMP Handle unknown timestamp
                // Will be removed when unknown timestamp is incorporated in architecture
                iLastDataTimestamp = 0;
            }
            else
            {
                PVMFSharedMediaMsgPtr msgPtr = iIncomingQueue.iQ.front();
                if (msgPtr->getFormatID() == PVMF_MEDIA_MSG_DATA_FORMAT_ID)
                {
                    PVMFSharedMediaDataPtr mediaData;
                    convertToPVMFMediaData(mediaData, msgPtr);
                    uint32 markerInfo = mediaData->getMarkerInfo();
                    if (markerInfo & PVMF_MEDIA_DATA_MARKER_INFO_DURATION_AVAILABLE_BIT)
                    {
                        iLastDataTimestamp += iIncomingQueue.iQ.front()->getDuration();
                    }
                }
            }
            iLastDataTimestampSet = true;
        }
        // Or the sync util's data queue
        else if (iDataQueue.DataQueue().size() > 0)
        {
            iLastDataTimestamp = iDataQueue.DataQueue()[0]->getTimestamp();
            if (iLastDataTimestamp == PVMF_MAX_TIME_STAMP)
            {
                // TEMP Handle unknown timestamp
                // Will be removed when unknown timestamp is incorporated in architecture
                iLastDataTimestamp = 0;
            }
            else
            {
                PVMFSharedMediaMsgPtr msgPtr = iDataQueue.DataQueue()[0];
                if (msgPtr->getFormatID() == PVMF_MEDIA_MSG_DATA_FORMAT_ID)
                {
                    PVMFSharedMediaDataPtr mediaData;
                    convertToPVMFMediaData(mediaData, msgPtr);
                    uint32 markerInfo = mediaData->getMarkerInfo();
                    if (markerInfo & PVMF_MEDIA_DATA_MARKER_INFO_DURATION_AVAILABLE_BIT)
                    {
                        iLastDataTimestamp += iDataQueue.DataQueue()[0]->getDuration();
                    }
                }
            }
            iLastDataTimestampSet = true;
        }
    }

    // Check if the incoming media data is already past the resume point
    if (iLastDataTimestampSet == true && aResumeTimestamp <= iLastDataTimestamp)
    {
        // Skip is already done so send the skip complete in Run
        iSkipAlreadyComplete = true;
        RunIfNotReady();

        // Return a command ID
        iSkipMediaDataContext = aContext;
        iSkipMediaDataSessionId = aSessionId;
        iSkipMediaDataCmdId = iNode->iCmdIdCounter;
        if (++(iNode->iCmdIdCounter) == PVMF_MAX_CMDID_CNTR)
        {
            iNode->iCmdIdCounter = 0;
        }
        return iSkipMediaDataCmdId;
    }

    // Wait for data to come in
    switch (iDataQueue.SkipMediaData(aResumeTimestamp, true))
    {
        case PVMFPending:
            iSkipResumeTimestamp = aResumeTimestamp;
            iSkipMediaDataPending = true;
            iSkipMediaDataContext = aContext;

            iSkipMediaDataSessionId = aSessionId;
            iSkipMediaDataCmdId = iNode->iCmdIdCounter;
            if (++(iNode->iCmdIdCounter) == PVMF_MAX_CMDID_CNTR)
            {
                iNode->iCmdIdCounter = 0;
            }
            return iSkipMediaDataCmdId;

        default:
            OSCL_LEAVE(OsclErrGeneral);
            return 0;
    }
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::ScheduleProcessData(PvmfSyncUtilDataQueue* aDataQueue, uint32 aTimeMilliseconds)
{
    OSCL_UNUSED_ARG(aDataQueue);
//  if(iSkipMediaDataPending || iState == PORT_STATE_STARTED)
    {
        RunIfNotReady(aTimeMilliseconds * 1000);
    }
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::SkipMediaDataComplete()
{
    iSkipResumeTimestamp = 0;
    //Report completion to the session observer.
    PVMFCmdResp resp(iSkipMediaDataCmdId, iSkipMediaDataContext, PVMFSuccess);
    iNode->ReportCmdCompleteEvent(iSkipMediaDataSessionId, resp);

    iSkipMediaDataPending = false;
    ScheduleProcessData(&iDataQueue, 0);
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::CancelSkipMediaData()
{
    iSkipResumeTimestamp = 0;
    iDataQueue.CancelSkipMediaData();
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::QueryInterface(const PVUuid &aUuid, OsclAny*&aPtr)
{
    if (aUuid == PVMI_CAPABILITY_AND_CONFIG_PVUUID)
    {
        aPtr = (PvmiCapabilityAndConfig*)this;
    }
    else
    {
        aPtr = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////
bool PVMFDummyFileOutputInPort::IsFormatSupported(PVMFFormatType aFmt)
{
    return ((aFmt == PVMF_MIME_AMR_IF2) ||
            (aFmt == PVMF_MIME_ADTS) ||
            (aFmt == PVMF_MIME_MPEG4_AUDIO) ||
            (aFmt == PVMF_MIME_LATM) ||
            (aFmt == PVMF_MIME_M4V) ||
            (aFmt == PVMF_MIME_H2632000) ||
            (aFmt == PVMF_MIME_PCM) ||
            (aFmt == PVMF_MIME_PCM8) ||
            (aFmt == PVMF_MIME_PCM16) ||
            (aFmt == PVMF_MIME_YUV420) ||
            (aFmt == PVMF_MIME_3GPP_TIMEDTEXT));
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::FormatUpdated()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO
                    , (0, "PVMFDummyFileOutputInPort::FormatUpdated %s", iFormat.getMIMEStrPtr()));
    //set port name for datapath logging
    if (iFormat.isAudio())
    {
        SetName("FileOutIn(Audio)");
        iDataQueue.SetName("FileOutIn(Audio)");
    }
    else if (iFormat.isVideo())
    {
        SetName("FileOutIn(Video)");
        iDataQueue.SetName("FileOutIn(Video)");
    }
    else if (iFormat.isText())
    {
        SetName("FileOutIn(Text)");
        iDataQueue.SetName("FileOutIn(Text)");
    }
    else
    {
        SetName("FileOutIn");
        iDataQueue.SetName("FileOutIn");
    }
}

////////////////////////////////////////////////////////////////////////////
//                  PvmiCapabilityAndConfig
////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void PVMFDummyFileOutputInPort::setObserver(PvmiConfigAndCapabilityCmdObserver* aObserver)
{
    // Not supported
    OSCL_UNUSED_ARG(aObserver);
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFStatus PVMFDummyFileOutputInPort::getParametersSync(PvmiMIOSession session,
        PvmiKeyType identifier,
        PvmiKvp*& parameters,
        int& num_parameter_elements,
        PvmiCapabilityContext context)
{
    LOG_STACK_TRACE((0, "PVMFDummyFileOutputInPort::getParametersSync"));
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(context);

    parameters = NULL;
    num_parameter_elements = 0;
    PVMFStatus status = PVMFFailure;

    if (pv_mime_strcmp(identifier, INPUT_FORMATS_CAP_QUERY) == 0)
    {
        num_parameter_elements = 15;
        status = AllocateKvp(parameters, OSCL_STATIC_CAST(PvmiKeyType, INPUT_FORMATS_VALTYPE), num_parameter_elements);
        if (status != PVMFSuccess)
        {
            LOG_ERR((0, "PVMFDummyFileOutputInPort::GetOutputParametersSync: Error - AllocateKvp failed. status=%d", status));
        }
        else
        {
            parameters[0].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_AMR_IETF);
            parameters[1].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_ADTS);
            parameters[2].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_MPEG4_AUDIO);
            parameters[3].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_LATM);
            parameters[4].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_M4V);
            parameters[5].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_H2632000);
            parameters[6].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_PCM8);
            parameters[7].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_PCM16);
            parameters[8].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_YUV420);
            parameters[9].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_AMR_IF2);
            parameters[10].value.pChar_value = OSCL_STATIC_CAST(char*, PVMF_MIME_3GPP_TIMEDTEXT);
        }
    }
    else if (pv_mime_strcmp(identifier, INPUT_FORMATS_CUR_QUERY) == 0)
    {
        num_parameter_elements = 1;
        status = AllocateKvp(parameters, OSCL_STATIC_CAST(PvmiKeyType, INPUT_FORMATS_VALTYPE), num_parameter_elements);
        if (status != PVMFSuccess)
        {
            LOG_ERR((0, "PVMFDummyFileOutputInPort::GetOutputParametersSync: Error - AllocateKvp failed. status=%d", status));
        }
        else
        {
            parameters[0].value.pChar_value = (char*)iFormat.getMIMEStrPtr();
        }
    }
    else if (pv_mime_strcmp(identifier, PVMF_FILE_OUTPUT_PORT_INPUT_FORMATS) == 0)
    {
        num_parameter_elements = 1;
        status = AllocateKvp(parameters, OSCL_STATIC_CAST(PvmiKeyType, PVMF_FILE_OUTPUT_PORT_INPUT_FORMATS_VALTYPE), num_parameter_elements);
        if (status != PVMFSuccess)
        {
            LOG_ERR((0, "PVMFDummyFileOutputInPort::GetOutputParametersSync: Error - AllocateKvp failed. status=%d", status));
        }
        else
        {
            parameters[0].value.pChar_value = (char*)iFormat.getMIMEStrPtr();
        }
    }

    return status;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFStatus PVMFDummyFileOutputInPort::releaseParameters(PvmiMIOSession session,
        PvmiKvp* parameters,
        int num_elements)
{
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(num_elements);

    if (parameters)
    {
        iAlloc.deallocate((OsclAny*)parameters);
        return PVMFSuccess;
    }
    else
    {
        return PVMFFailure;
    }
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void PVMFDummyFileOutputInPort::createContext(PvmiMIOSession session, PvmiCapabilityContext& context)
{
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(context);
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void PVMFDummyFileOutputInPort::setContextParameters(PvmiMIOSession session,
        PvmiCapabilityContext& context,
        PvmiKvp* parameters, int num_parameter_elements)
{
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(context);
    OSCL_UNUSED_ARG(parameters);
    OSCL_UNUSED_ARG(num_parameter_elements);
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void PVMFDummyFileOutputInPort::DeleteContext(PvmiMIOSession session, PvmiCapabilityContext& context)
{
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(context);
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF void PVMFDummyFileOutputInPort::setParametersSync(PvmiMIOSession session, PvmiKvp* parameters,
        int num_elements, PvmiKvp*& ret_kvp)
{
    OSCL_UNUSED_ARG(session);
    PVMFStatus status = PVMFSuccess;
    ret_kvp = NULL;

    for (int32 i = 0; i < num_elements; i++)
    {
        status = VerifyAndSetParameter(&(parameters[i]), true);
        if (status != PVMFSuccess)
        {
            LOG_ERR((0, "PVMFDummyFileOutputInPort::setParametersSync: Error - VerifiyAndSetParameter failed on parameter #%d", i));
            ret_kvp = &(parameters[i]);
            OSCL_LEAVE(OsclErrArgument);
        }
    }
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFCommandId PVMFDummyFileOutputInPort::setParametersAsync(PvmiMIOSession session,
        PvmiKvp* parameters,
        int num_elements,
        PvmiKvp*& ret_kvp,
        OsclAny* context)
{
    OSCL_UNUSED_ARG(session);
    OSCL_UNUSED_ARG(parameters);
    OSCL_UNUSED_ARG(num_elements);
    OSCL_UNUSED_ARG(ret_kvp);
    OSCL_UNUSED_ARG(context);
    return -1;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF uint32 PVMFDummyFileOutputInPort::getCapabilityMetric(PvmiMIOSession session)
{
    OSCL_UNUSED_ARG(session);
    return 0;
}

////////////////////////////////////////////////////////////////////////////
OSCL_EXPORT_REF PVMFStatus PVMFDummyFileOutputInPort::verifyParametersSync(PvmiMIOSession session,
        PvmiKvp* parameters, int num_elements)
{
    OSCL_UNUSED_ARG(session);

    PVMFStatus status = PVMFSuccess;
    for (int32 i = 0; (i < num_elements) && (status == PVMFSuccess); i++)
        status = VerifyAndSetParameter(&(parameters[i]));

    return status;
}

////////////////////////////////////////////////////////////////////////////
PVMFDummyFileOutputInPort::PVMFDummyFileOutputInPort(int32 aTag
        , PVMFNodeInterface* aNode)
        : PvmfPortBaseImpl(aTag, aNode, 0, 0, 0, 0, 0, 0, "PVFileOPPortIn"),
        OsclTimerObject(OsclActiveObject::EPriorityNominal, "PVMFDummyFileOutputInPort"),
        iDataQueue(this, &iSyncUtil),
        iNode(NULL),
        iSkipMediaDataPending(false),
        iSkipMediaDataCmdId(0),
        iSkipMediaDataContext(NULL),
        iExtensionRefCount(0),
        iState(PORT_STATE_BUFFERING),
        iFormat(PVMF_MIME_FORMAT_UNKNOWN),
        iSkipAlreadyComplete(false),
        iSkipResumeTimestamp(0),
        iLastDataTimestampSet(false),
        iLastDataTimestamp(0)
{
    ConstructL(aNode);
#if (PVMF_PORT_BASE_IMPL_STATS)
    oscl_memset((void*)&(PvmfPortBaseImpl::iStats), 0, sizeof(PvmfPortBaseImplStats));
#endif
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::ConstructL(PVMFNodeInterface* aContainerNode)
{
    if (!aContainerNode)
        OSCL_LEAVE(OsclErrArgument);

    iNode = (PVMFDummyFileOutputNode*)aContainerNode;
    AddToScheduler();
}

////////////////////////////////////////////////////////////////////////////
void PVMFDummyFileOutputInPort::Run()
{
    if (iSkipAlreadyComplete)
    {
        iSkipAlreadyComplete = false;
        SkipMediaDataComplete();
    }

    if (!iSkipMediaDataPending && iState == PORT_STATE_BUFFERING)
    {
        return;
    }

    PVMFSharedMediaMsgPtr mediamsgptr;
    {
        uint32 dropped;
        uint32 skipped;
        PVMFStatus retval = iDataQueue.DequeueMediaData(mediamsgptr, &dropped, &skipped);
        if (dropped > 0)
        {
            iNode->PVMFNodeInterface::ReportInfoEvent(PVMFInfoDataDiscarded);
        }

        if (retval == PVMFSuccess)
        {
            //schedule next processing.  this may wakeup one time too many, but
            //there's no good way to evaluate at this point whether we need
            //to process more data or not.
            if (!iDataQueue.DataQueue().empty())
            {
                RunIfNotReady();
            }

            PVMFTimestamp datatailtime = mediamsgptr->getTimestamp();
            if (mediamsgptr->getFormatID() == PVMF_MEDIA_MSG_DATA_FORMAT_ID)
            {
                PVMFSharedMediaDataPtr mediaData;
                convertToPVMFMediaData(mediaData, mediamsgptr);
                uint32 markerInfo = mediaData->getMarkerInfo();
                if (markerInfo & PVMF_MEDIA_DATA_MARKER_INFO_DURATION_AVAILABLE_BIT)
                {
                    datatailtime += mediamsgptr->getDuration();
                }
            }
            iLastDataTimestamp = datatailtime;
            iLastDataTimestampSet = true;

            if (mediamsgptr->getFormatID() == PVMF_MEDIA_CMD_EOS_FORMAT_ID)
            {
                // If skip media data is pending, complete it
                if (iSkipMediaDataPending)
                {
                    if (datatailtime >= iSkipResumeTimestamp)
                    {
                        LOG_STACK_TRACE((0, "PVMFDummyFileOutputInPort::Run In middle of skip so cancelling skip and sending out PVMFInfoEndOfData in response to EOS media cmd"));
                        // Cancel and complete the skip
                        CancelSkipMediaData();
                        SkipMediaDataComplete();
                        // Send out EndOfData event
                        iNode->ReportInfoEvent(PVMFInfoEndOfData);
                    }
                    else
                    {
                        // Ignore this EOS associated with data to be flushed
                        LOG_STACK_TRACE((0, "PVMFDummyFileOutputInPort::Run EOS being ignored since it is to be flushed in SkipMediaData"));
                    }
                }
                else
                {
                    // Send EndOfData event
                    LOG_STACK_TRACE((0, "PVMFDummyFileOutputInPort::Run Sending out PVMFInfoEndOfData in response to EOS media cmd"));
                    iNode->ReportInfoEvent(PVMFInfoEndOfData);
                }
            }
            else
            {
                // This is media data that is in-sync.
                // Check if it needs to be dropped when skipping
                if (iSkipMediaDataPending)
                {
                    if (datatailtime < iSkipResumeTimestamp)
                    {
                        // Data needs to be dropped
                        return;
                    }
                }

                // Write out the data
                PVMFSharedMediaDataPtr mediaDataPtr;
                convertToPVMFMediaData(mediaDataPtr, mediamsgptr);
                PVMFStatus status = iNode->ProcessIncomingData(mediaDataPtr);
                if (status != PVMFSuccess)
                {
                    iNode->iOutputFile.Flush();
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PVMFDummyFileOutputInPort::AllocateKvp(PvmiKvp*& aKvp, PvmiKeyType aKey, int32 aNumParams)
{
    LOG_STACK_TRACE((0, "PVMFDummyFileOutputInPort::AllocateKvp"));
    uint8* buf = NULL;
    uint32 keyLen = oscl_strlen(aKey) + 1;
    int32 err = 0;

    OSCL_TRY(err,
             buf = (uint8*)iAlloc.allocate(aNumParams * (sizeof(PvmiKvp) + keyLen));
             if (!buf)
             OSCL_LEAVE(OsclErrNoMemory);
            );
    OSCL_FIRST_CATCH_ANY(err,
                         LOG_ERR((0, "PVMFDummyFileOutputInPort::AllocateKvp: Error - kvp allocation failed"));
                         return PVMFErrNoMemory;
                        );

    int32 i = 0;
    PvmiKvp* curKvp = aKvp = OSCL_PLACEMENT_NEW(buf, PvmiKvp);
    buf += sizeof(PvmiKvp);
    for (i = 1; i < aNumParams; i++)
    {
        curKvp += i;
        curKvp = OSCL_PLACEMENT_NEW(buf, PvmiKvp);
        buf += sizeof(PvmiKvp);
    }

    for (i = 0; i < aNumParams; i++)
    {
        aKvp[i].key = (char*)buf;
        oscl_strncpy(aKvp[i].key, aKey, keyLen);
        buf += keyLen;
    }

    return PVMFSuccess;
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PVMFDummyFileOutputInPort::VerifyAndSetParameter(PvmiKvp* aKvp, bool aSetParam)
{
    LOG_STACK_TRACE((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: aKvp=0x%x, aSetParam=%d", aKvp, aSetParam));

    if (!aKvp)
    {
        LOG_ERR((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: Error - Invalid key-value pair"));
        return PVMFFailure;
    }

    if (pv_mime_strcmp(aKvp->key, INPUT_FORMATS_VALTYPE) == 0)
    {
        if (IsFormatSupported(aKvp->value.pChar_value))
        {
            if (aSetParam)
                iFormat = aKvp->value.pChar_value;
            return PVMFSuccess;
        }
        else
        {
            return PVMFErrNotSupported;
        }
    }
    else if (pv_mime_strcmp(aKvp->key, PVMF_FILE_OUTPUT_PORT_INPUT_FORMATS_VALTYPE) == 0)
    {
        if (IsFormatSupported(aKvp->value.pChar_value))
        {
            if (aSetParam)
                iFormat = aKvp->value.pChar_value;
            return PVMFSuccess;
        }
        else
        {
            return PVMFErrNotSupported;
        }
    }
    else if (pv_mime_strcmp(aKvp->key, MOUT_AUDIO_FORMAT_KEY) == 0)
    {
        LOG_DEBUG((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: Got audio format key"));
        // just return success for now
        return PVMFSuccess;
    }
    else if (pv_mime_strcmp(aKvp->key, MOUT_AUDIO_SAMPLING_RATE_KEY) == 0)
    {
        LOG_DEBUG((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: Audio sample rate=%d", aKvp->value.uint32_value));
        // just return success for now
        return PVMFSuccess;
    }
    else if (pv_mime_strcmp(aKvp->key, MOUT_AUDIO_NUM_CHANNELS_KEY) == 0)
    {
        LOG_DEBUG((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: Audio num channels=%d", aKvp->value.uint32_value));
        // just return success for now
        return PVMFSuccess;
    }
    else if (pv_mime_strcmp(aKvp->key, PVMF_FORMAT_SPECIFIC_INFO_KEY) == 0)
    {
        PVMFStatus status =
            iNode->WriteFormatSpecificInfo((OsclAny*)(aKvp->value.pChar_value), aKvp->capacity);

        if (status != PVMFSuccess)
        {
            LOG_ERR((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: WriteFormatSpecificInfo Error"));
        }
        return status;
    }
    else if (pv_mime_strcmp(aKvp->key, PVMF_BITRATE_VALUE_KEY) == 0)
    {
        LOG_DEBUG((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: bitrate=%d", aKvp->value.uint32_value));
        // just return success for now
        return PVMFSuccess;
    }
    else if (pv_mime_strcmp(aKvp->key, PVMF_FRAMERATE_VALUE_KEY) == 0)
    {
        LOG_DEBUG((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: framerate=%d", aKvp->value.uint32_value));
        // just return success for now
        return PVMFSuccess;
    }

    LOG_ERR((0, "PVMFDummyFileOutputInPort::VerifyAndSetParameter: Error - Unsupported parameter"));
    return PVMFFailure;
}
