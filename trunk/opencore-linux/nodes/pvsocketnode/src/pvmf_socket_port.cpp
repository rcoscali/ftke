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
#ifndef PVMF_SOCKET_PORT_H_INCLUDED
#include "pvmf_socket_port.h"
#endif

#ifndef OSCL_MIME_STRING_UTILS_H
#include "pv_mime_string_utils.h"
#endif

#include "pvmf_socket_node_tunables.h"


PVMFSocketPort::PVMFSocketPort(int32 aTag, PVMFNodeInterface* aNode)
        : PvmfPortBaseImpl(aTag, aNode)
{
    Construct();
}

////////////////////////////////////////////////////////////////////////////
PVMFSocketPort::PVMFSocketPort(int32 aTag, PVMFNodeInterface* aNode
                               , uint32 aInCapacity
                               , uint32 aInReserve
                               , uint32 aInThreshold
                               , uint32 aOutCapacity
                               , uint32 aOutReserve
                               , uint32 aOutThreshold)
        : PvmfPortBaseImpl(aTag, aNode, aInCapacity, aInReserve, aInThreshold, aOutCapacity, aOutReserve, aOutThreshold)
{
    iPortTag = (PVMFSocketNodePortTag)aTag;
    Construct();
}

////////////////////////////////////////////////////////////////////////////
void PVMFSocketPort::Construct()
{
    iConfig = NULL;
    iLogger = PVLogger::GetLoggerObject("PVMFSocketPort");
    oscl_memset(&iStats, 0, sizeof(PvmfPortBaseImplStats));
    iNumFramesConsumed = 0;
    PvmiCapabilityAndConfigPortFormatImpl::Construct(
        PVMF_SOCKET_PORT_SPECIFIC_ALLOCATOR
        , PVMF_SOCKET_PORT_SPECIFIC_ALLOCATOR_VALTYPE);
}

////////////////////////////////////////////////////////////////////////////
PVMFSocketPort::~PVMFSocketPort()
{
    Disconnect();
}

////////////////////////////////////////////////////////////////////////////
bool PVMFSocketPort::IsFormatSupported(PVMFFormatType aFmt)
{
    bool formatSupported = false;
    if ((aFmt == PVMF_MIME_INET_UDP) || (aFmt == PVMF_MIME_INET_TCP))
    {
        formatSupported = true;
    }
    return formatSupported;
}

////////////////////////////////////////////////////////////////////////////
void PVMFSocketPort::FormatUpdated()
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO
                    , (0, "PVMFSocketPort::FormatUpdated %s", iFormat.getMIMEStrPtr()));
}

void PVMFSocketPort::setParametersSync(PvmiMIOSession aSession,
                                       PvmiKvp* aParameters,
                                       int num_elements,
                                       PvmiKvp * & aRet_kvp)
{
    OSCL_UNUSED_ARG(aSession);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO, (0, "PVMFSocketPort::getParametersSync: aSession=0x%x, aParameters=0x%x, num_elements=%d, aRet_kvp=0x%x",
                    aSession, aParameters, num_elements, aRet_kvp));

    if (!aParameters || (num_elements != 1) ||
            (pv_mime_strcmp(aParameters->key, PVMF_SOCKET_PORT_SPECIFIC_ALLOCATOR_VALTYPE) != 0))
    {
        aRet_kvp = aParameters;
        OSCL_LEAVE(OsclErrArgument);
    }
    if (aParameters->value.key_specific_value == NULL)
    {
        aRet_kvp = aParameters;
        OSCL_LEAVE(OsclErrArgument);
    }
}

PVMFStatus PVMFSocketPort::getParametersSync(PvmiMIOSession aSession,
        PvmiKeyType aIdentifier,
        PvmiKvp*& aParameters,
        int& num_parameter_elements,
        PvmiCapabilityContext aContext)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO, (0, "PVMFSocketPort::getParametersSync: aSession=0x%x, aIdentifier=%s, aParameters=0x%x, num_parameters_elements=%d, aContext=0x%x",
                    aSession, aIdentifier, aParameters, num_parameter_elements, aContext));

    if (pv_mime_strcmp(aIdentifier, PVMI_PORT_CONFIG_INPLACE_DATA_PROCESSING_KEY) != 0)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFSocketPort::getParametersSync: Error - Unsupported PvmiKeyType"));
        return PVMFErrNotSupported;
    }

    num_parameter_elements = 0;
    if (!pvmiGetPortInPlaceDataProcessingInfoSync(PVMI_PORT_CONFIG_INPLACE_DATA_PROCESSING_VALUE, aParameters))
    {
        return PVMFFailure;
    }
    num_parameter_elements = 1;
    return PVMFSuccess;
}

PVMFStatus PVMFSocketPort::releaseParameters(PvmiMIOSession aSession,
        PvmiKvp* aParameters,
        int num_elements)
{
    OSCL_UNUSED_ARG(aSession);
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO, (0, "PVMFSocketPort::releaseParameters: aSession=0x%x, aParameters=0x%x, num_elements=%d",
                    aSession, aParameters, num_elements));

    if ((num_elements != 1) ||
            (pv_mime_strcmp(aParameters->key, PVMI_PORT_CONFIG_INPLACE_DATA_PROCESSING_VALUE) != 0))
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFSocketPort::releaseParameters: Error - Not a PvmiKvp created by this port"));
        return PVMFFailure;
    }
    OsclMemAllocator alloc;
    alloc.deallocate((OsclAny*)(aParameters));
    return PVMFSuccess;
}

bool
PVMFSocketPort::pvmiGetPortInPlaceDataProcessingInfoSync(const char* aFormatValType,
        PvmiKvp*& aKvp)
{
    /*
     * Create PvmiKvp for capability settings
     */
    aKvp = NULL;
    OsclMemAllocator alloc;
    uint32 strLen = oscl_strlen(aFormatValType) + 1;
    uint8* ptr = (uint8*)alloc.allocate(sizeof(PvmiKvp) + strLen);
    if (!ptr)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFSocketPort::pvmiGetPortInPlaceDataProcessingInfoSync: Error - No memory. Cannot allocate PvmiKvp"));
        return false;
    }
    aKvp = new(ptr) PvmiKvp;
    ptr += sizeof(PvmiKvp);
    aKvp->key = (PvmiKeyType)ptr;
    oscl_strncpy(aKvp->key, aFormatValType, strLen);
    aKvp->length = aKvp->capacity = strLen;

#if SNODE_ENABLE_UDP_MULTI_PACKET
    if (iTag == PVMF_SOCKET_NODE_PORT_TYPE_SOURCE)
        aKvp->value.bool_value = false;//for the multiple UDP recv feature
    else
#endif
        aKvp->value.bool_value = true;

    return true;
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PVMFSocketPort::PeekIncomingMsg(PVMFSharedMediaMsgPtr& aMsg)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE,
                    (0, "0x%x PVMFSocketPort::PeekIncomingMsg", this));

    if (iIncomingQueue.iQ.empty())
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_REL, iLogger, PVLOGMSG_ERR,
                        (0, "0x%x PVMFSocketPort::PeekIncomingMsg: Error - Incoming queue is empty", this));
        return PVMFFailure;
    }

    // Save message to output parameter and erase it from queue
    aMsg = iIncomingQueue.iQ.front();

    return PVMFSuccess;
}

////////////////////////////////////////////////////////////////////////////
PVMFStatus PVMFSocketPort::QueueOutgoingMsg(PVMFSharedMediaMsgPtr aMsg)
//In this node, we skip the outgoing queue and go directly to the connected port's
//incoming queue.
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO, (0, "PVMFSocketPort::QueueOutgoingMsg"));

    //If port is not connected, don't accept data on the
    //outgoing queue.
    if (!iConnectedPort)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO, (0, "PVMFSocketPort::QueueOutgoingMsg: Error - Port not connected"));
        return PVMFFailure;
    }

    PvmfPortBaseImpl* cpPort = OSCL_STATIC_CAST(PvmfPortBaseImpl*, iConnectedPort);

    // Connected Port incoming Queue is in busy / flushing state.  Do not accept more outgoing messages
    // until the queue is not busy, i.e. queue size drops below specified threshold or FlushComplete
    // is called.
    if (cpPort->iIncomingQueue.iBusy)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO, (0, "PVMFSocketPort::QueueOutgoingMsg: Connected Port Incoming queue in busy / flushing state - Attempting to Q in output port's outgoing msg q"));
        return (PvmfPortBaseImpl::QueueOutgoingMsg(aMsg));
    }

    // Add message to outgoing queue and notify the node of the activity
    // There is no need to trap the push_back, since it cannot leave in this usage
    // Reason being that we do a reserve in the constructor and we do not let the
    // port queues grow indefinitely (we either a connected port busy or outgoing Q busy
    // before we reach the reserved limit
    PVMFStatus status = cpPort->Receive(aMsg);

    if (status != PVMFSuccess)
    {
        return PVMFFailure;
    }

    // Outgoing queue size is at capacity and goes into busy state. The owner node is
    // notified of this transition into busy state.
    if (cpPort->isIncomingFull())
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_MLDBG, iLogger, PVLOGMSG_INFO, (0, "PVMFSocketPort::QueueOutgoingMsg: Connected Port incoming queue is full. Goes into busy state"));
        cpPort->iIncomingQueue.iBusy = true;
        PvmfPortBaseImpl::PortActivity(PVMF_PORT_ACTIVITY_CONNECTED_PORT_BUSY);
    }

    return PVMFSuccess;
}

bool PVMFSocketPort::IsOutgoingQueueBusy()
//In this node, we skip the outgoing queue and go directly to the connected port's
//incoming queue.
{
    if (iConnectedPort != NULL)
    {
        PvmfPortBaseImpl* cpPort = OSCL_STATIC_CAST(PvmfPortBaseImpl*, iConnectedPort);
        return (cpPort->iIncomingQueue.iBusy);
    }
    return (PvmfPortBaseImpl::IsOutgoingQueueBusy());;
}

