/*
 * Copyright (c) 2008, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_AUDIO_INPUT_H_INCLUDED
#define ANDROID_AUDIO_INPUT_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif
#ifndef OSCLCONFIG_IO_H_INCLUDED
#include "osclconfig_io.h"
#endif
#ifndef OSCL_STRING_H_INCLUDED
#include "oscl_string.h"
#endif
#ifndef OSCL_FILE_IO_H_INCLUDED
#include "oscl_file_io.h"
#endif
#ifndef OSCL_MEM_MEMPOOL_H_INCLUDED
#include "oscl_mem_mempool.h"
#endif
#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif
#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif
#ifndef PVMI_MIO_CONTROL_H_INCLUDED
#include "pvmi_mio_control.h"
#endif
#ifndef PVMI_MEDIA_TRANSFER_H_INCLUDED
#include "pvmi_media_transfer.h"
#endif
#ifndef PVMI_CONFIG_AND_CAPABILITY_H_INCLUDED
#include "pvmi_config_and_capability.h"
#endif
#ifndef PVMF_SIMPLE_MEDIA_BUFFER_H_INCLUDED
#include "pvmf_simple_media_buffer.h"
#endif
#ifndef ANDROID_AUDIO_INPUT_THREADSAFE_CALLBACK_AO_H_INCLUDED
#include "android_audio_input_threadsafe_callbacks.h"
#endif
#ifndef PVMF_MEDIA_CLOCK_H_INCLUDED
#include "pvmf_media_clock.h"
#endif

#include <utils/RefBase.h>

#ifdef HIDE_MIO_SYMBOLS
#pragma GCC visibility push(hidden)
#endif

namespace android {

static const int32 DEFAULT_AUDIO_NUMBER_OF_CHANNELS = 1;
static const int32 DEFAULT_AUDIO_SAMPLING_RATE = 8000;

static const int32 MAX_AUDIO_SAMPLING_RATE = 96000; // In Hz
static const int32 MIN_AUDIO_SAMPLING_RATE = 7350;  // In Hz

static const int32 MAX_AUDIO_NUMBER_OF_CHANNELS = 2;
static const int32 MIN_AUDIO_NUMBER_OF_CHANNELS = 1;


class AndroidAudioInput;
class Mutex;
class Condition;

/**
 * Enumerated list of asychronous commands for AndroidAudioInput
 */
typedef enum
{
    AI_CMD_QUERY_UUID,
    AI_CMD_QUERY_INTERFACE,
    AI_CMD_INIT,
    AI_CMD_START,
    AI_CMD_PAUSE,
    AI_CMD_FLUSH,
    AI_CMD_STOP,
    AI_CMD_CANCEL_ALL_COMMANDS,
    AI_CMD_CANCEL_COMMAND,
    AI_CMD_RESET,
    AI_DATA_READ_EVENT,
    AI_DATA_WRITE_EVENT,
    AI_INVALID_CMD
} AndroidAudioInputCmdType;

//Write command handling
// the audio output thread queues the responses, this mio dequeues and processes them
class WriteResponse
{
public:
    WriteResponse(PVMFStatus s,PVMFCommandId id,const OsclAny* ctx,const PVMFTimestamp& ts)
            :iStatus(s),iCmdId(id),iContext(ctx),iTimestamp(ts)
    {}

        PVMFStatus iStatus;
        PVMFCommandId iCmdId;
        const OsclAny* iContext;
        PVMFTimestamp iTimestamp;
};

/**
 * Class containing information for a command or data event
 */
class AndroidAudioInputCmd
{
public:
    AndroidAudioInputCmd()
    {
        iId = 0;
        iType = AI_INVALID_CMD;
        iContext = NULL;
        iData1 = NULL;
    }

    AndroidAudioInputCmd(const AndroidAudioInputCmd& aCmd)
    {
        Copy(aCmd);
    }

    ~AndroidAudioInputCmd() {}

    AndroidAudioInputCmd& operator=(const AndroidAudioInputCmd& aCmd)
    {
        Copy(aCmd);
        return (*this);
    }

    PVMFCommandId iId; /** ID assigned to this command */
    int32 iType;  /** AndroidAudioInputCmdType value */
    OsclAny* iContext;  /** Other data associated with this command */
    OsclAny* iData1;  /** Other data associated with this command */

private:

    void Copy(const AndroidAudioInputCmd& aCmd)
    {
        iId = aCmd.iId;
        iType = aCmd.iType;
        iContext = aCmd.iContext;
        iData1 = aCmd.iData1;
    }
};

class AndroidAudioInputMediaData
{
public:
    AndroidAudioInputMediaData()
    {
        iId = 0;
        iData = NULL;
    }

    AndroidAudioInputMediaData(const AndroidAudioInputMediaData& aData)
    {
        iId = aData.iId;
        iData = aData.iData;
    }

    PVMFCommandId iId;
    OsclAny* iData;
};


//Audio output request handling
// this mio queues the requests, the audio output thread dequeues and processes them
class OSSRequest
{
public:
    OSSRequest(uint8* data, uint32 len,PVMFCommandId id,const OsclAny* ctx,const PVMFTimestamp& ts)
            :iData(data),iDataLen(len),iCmdId(id),iContext(ctx),iTimestamp(ts)
    {}
        uint8* iData;
        uint32 iDataLen;
        PVMFCommandId iCmdId;
        const OsclAny* iContext;
        PVMFTimestamp iTimestamp;
};

class MicData
{
public:
    MicData(uint8* data, uint32 len, PVMFTimestamp& ts, int32 duration)
        :iData(data),iDataLen(len),iTimestamp(ts),iDuration(duration)
    {}
    uint8* iData;
    uint32 iDataLen;
    PVMFTimestamp iTimestamp;
    int32 iDuration;
};


class AndroidAudioInput : public OsclTimerObject,
    public PvmiMIOControl,
    public PvmiMediaTransfer,
    public PvmiCapabilityAndConfig,
    public RefBase,
    public PVMFMediaClockStateObserver
{
public:
    AndroidAudioInput(uint32 audioSource);
    virtual ~AndroidAudioInput();

    // Pure virtuals from PvmiMIOControl
    PVMFStatus connect(PvmiMIOSession& aSession, PvmiMIOObserver* aObserver);
    PVMFStatus disconnect(PvmiMIOSession aSession);
    PvmiMediaTransfer* createMediaTransfer(PvmiMIOSession& aSession,
                                                         PvmiKvp* read_formats=NULL,
                                                         int32 read_flags=0,
                                                         PvmiKvp* write_formats=NULL,
                                                         int32 write_flags=0);
    void deleteMediaTransfer(PvmiMIOSession& aSession,
                                           PvmiMediaTransfer* media_transfer);
    PVMFCommandId QueryUUID(const PvmfMimeString& aMimeType,
                                          Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids,
                                          bool aExactUuidsOnly=false,
                                          const OsclAny* aContext=NULL);
    PVMFCommandId QueryInterface(const PVUuid& aUuid,
                                               PVInterface*& aInterfacePtr,
                                               const OsclAny* aContext=NULL);
    PVMFCommandId Init(const OsclAny* aContext=NULL);
    PVMFCommandId Start(const OsclAny* aContext=NULL);
    PVMFCommandId Reset(const OsclAny* aContext=NULL);
    PVMFCommandId Pause(const OsclAny* aContext=NULL);
    PVMFCommandId Flush(const OsclAny* aContext=NULL);
    PVMFCommandId DiscardData(PVMFTimestamp aTimestamp, const OsclAny* aContext=NULL);
    PVMFCommandId DiscardData(const OsclAny* aContext=NULL);
    PVMFCommandId Stop(const OsclAny* aContext=NULL);
    PVMFCommandId CancelCommand(PVMFCommandId aCmdId, const OsclAny* aContext=NULL);
    PVMFCommandId CancelAllCommands(const OsclAny* aContext=NULL);
    void ThreadLogon();
    void ThreadLogoff();

    // Pure virtuals from PvmiMediaTransfer
    void setPeer(PvmiMediaTransfer* aPeer);
    void useMemoryAllocators(OsclMemAllocator* write_alloc=NULL);
    PVMFCommandId writeAsync(uint8 format_type, int32 format_index,
                                           uint8* data, uint32 data_len,
                                           const PvmiMediaXferHeader& data_header_info,
                                           OsclAny* aContext=NULL);
    void writeComplete(PVMFStatus aStatus, PVMFCommandId write_cmd_id,
                                     OsclAny* aContext);
    PVMFCommandId readAsync(uint8* data, uint32 max_data_len, OsclAny* aContext=NULL,
                                          int32* formats=NULL, uint16 num_formats=0);
    void readComplete(PVMFStatus aStatus, PVMFCommandId read_cmd_id,
                                    int32 format_index,
                                    const PvmiMediaXferHeader& data_header_info,
                                    OsclAny* aContext);
    void statusUpdate(uint32 status_flags);
    void cancelCommand(PVMFCommandId aCmdId);
    void cancelAllCommands();

    // Pure virtuals from PvmiCapabilityAndConfig
    void setObserver (PvmiConfigAndCapabilityCmdObserver* aObserver);
    PVMFStatus getParametersSync(PvmiMIOSession aSession, PvmiKeyType aIdentifier,
                                               PvmiKvp*& aParameters, int& num_parameter_elements,
                                               PvmiCapabilityContext aContext);
    PVMFStatus releaseParameters(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                               int num_elements);
    void createContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext);
    void setContextParameters(PvmiMIOSession aSession, PvmiCapabilityContext& aContext,
                                            PvmiKvp* aParameters, int num_parameter_elements);
    void DeleteContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext);
    void setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                         int num_elements, PvmiKvp * & aRet_kvp);
    PVMFCommandId setParametersAsync(PvmiMIOSession aSession, PvmiKvp* aParameters,
                                                   int num_elements, PvmiKvp*& aRet_kvp,
                                                   OsclAny* context=NULL);
    uint32 getCapabilityMetric (PvmiMIOSession aSession);
    PVMFStatus verifyParametersSync (PvmiMIOSession aSession,
                                                   PvmiKvp* aParameters, int num_elements);

    // Android-specific stuff

    /* Returns the max absolute amplitude since the last call of this function. Used for
     * visualization.
     */
    int maxAmplitude();

    /* Sets the input sampling rate */
    bool setAudioSamplingRate(int32 iSamplingRate);

    /* Set the input number of channels */
    bool setAudioNumChannels(int32 iNumChannels);

    /* Sets the audio input source */
    bool setAudioSource(uint32 iSource);

    /* From PVMFMediaClockStateObserver and its base*/
    void ClockStateUpdated();
    void NotificationsInterfaceDestroyed();

private:
    AndroidAudioInput();
    void Run();

    int audin_thread_func();
    static int start_audin_thread_func(TOsclThreadFuncArg arg);
    void SendMicData(void);

    PVMFCommandId AddCmdToQueue(AndroidAudioInputCmdType aType, const OsclAny* aContext, OsclAny* aData1 = NULL);
    void AddDataEventToQueue(uint32 aMicroSecondsToEvent);
    void DoRequestCompleted(const AndroidAudioInputCmd& aCmd, PVMFStatus aStatus, OsclAny* aEventData=NULL);
    PVMFStatus DoInit();
    PVMFStatus DoStart();
    PVMFStatus DoReset();
    PVMFStatus DoPause();
    PVMFStatus DoFlush();
    PVMFStatus DoStop();
    PVMFStatus DoRead();

    /**
     * Allocate a specified number of key-value pairs and set the keys
     *
     * @param aKvp Output parameter to hold the allocated key-value pairs
     * @param aKey Key for the allocated key-value pairs
     * @param aNumParams Number of key-value pairs to be allocated
     * @return Completion status
     */
    PVMFStatus AllocateKvp(PvmiKvp*& aKvp, PvmiKeyType aKey, int32 aNumParams);

    /**
     * Verify one key-value pair parameter against capability of the port and
     * if the aSetParam flag is set, set the value of the parameter corresponding to
     * the key.
     *
     * @param aKvp Key-value pair parameter to be verified
     * @param aSetParam If true, set the value of parameter corresponding to the key.
     * @return PVMFSuccess if parameter is supported, else PVMFFailure
     */
    PVMFStatus VerifyAndSetParameter(PvmiKvp* aKvp, bool aSetParam=false);

    // Linearly ramp up the volume from 0 to full over a duration given
    // in "kAutoRampDurationFrames". The time of the first audio frame
    // passed in is "timeInFrames".
    void RampVolume(int32 timeInFrames, int32 kAutoRampDurationFrames,
                    void *_data, size_t numBytes) const;

    void RemoveDestroyClockStateObs();

    // Command queue
    uint32 iCmdIdCounter;
    Oscl_Vector<AndroidAudioInputCmd, OsclMemAllocator> iCmdQueue;

    // PvmiMIO sessions
    Oscl_Vector<PvmiMIOObserver*, OsclMemAllocator> iObservers;

    PvmiMediaTransfer* iPeer;

    // Thread logon
    bool iThreadLoggedOn;

    // semaphore used to communicate with the audio input thread
    OsclSemaphore* iAudioThreadSem;
    // and another one
    OsclSemaphore* iAudioThreadTermSem;

    volatile bool iExitAudioThread;
    bool iWriteBusy;
    uint32 iWriteBusySeqNum;

    PvmiMIOObserver* iObserver;


    uint32 iCommandCounter;

    //Control command handling.
    class CommandResponse
    {
    public:
        CommandResponse(PVMFStatus s,PVMFCommandId id,const OsclAny* ctx)
            :iStatus(s),iCmdId(id),iContext(ctx)
        {}

        PVMFStatus iStatus;
        PVMFCommandId iCmdId;
        const OsclAny* iContext;
    };
    Oscl_Vector<CommandResponse,OsclMemAllocator> iCommandResponseQueue;

    //Audio parameters.
    OSCL_HeapString<OsclMemAllocator> iAudioFormatString;
    PVMFFormatType iAudioFormat;
    int32 iAudioNumChannels;
    int32 iAudioSamplingRate;
    uint32 iAudioSource;

    int32 iFrameSize;
    int32 iDataEventCounter;

    // Functions specific to this MIO

    //request active object which the audio output thread uses to schedule this timer object to run
    AndroidAudioInputThreadSafeCallbackAO *iWriteCompleteAO;

    // write response queue, needs to use lock mechanism to access
    //Oscl_Vector<WriteResponse,OsclMemAllocator> iWriteResponseQueue;
    Oscl_Vector<MicData,OsclMemAllocator> iWriteResponseQueue;

    // lock used to access the write response queue
    OsclMutex iWriteResponseQueueLock;

    // oss request queue, needs to use lock mechanism to access
    //Oscl_Vector<OSSRequest,OsclMemAllocator> iOSSRequestQueue;
    Oscl_Vector<uint8*, OsclMemAllocator> iOSSRequestQueue;

    // lock used to access the oss request queue
    OsclMutex iOSSRequestQueueLock;

    // Timing
    int32 iMilliSecondsPerDataEvent;
    int32 iMicroSecondsPerDataEvent;
    PVMFTimestamp iTimeStamp;

    // Allocator for simple media data buffer
    OsclMemAllocator iAlloc;
    OsclMemPoolFixedChunkAllocator* iMediaBufferMemPool;

    Oscl_Vector<AndroidAudioInputMediaData, OsclMemAllocator> iSentMediaData;

    // State machine
    enum AndroidAudioInputState
    {
        STATE_IDLE,
        STATE_INITIALIZED,
        STATE_STARTED,
        STATE_FLUSHING,
        STATE_PAUSED,
        STATE_STOPPED
    };

    AndroidAudioInputState iState;

    FILE *fp;

    volatile int iMaxAmplitude;
    volatile bool iTrackMaxAmplitude;

    // synchronize startup of audio input thread, so we can return an error
    // from DoStart() if something goes wrong
    Mutex *iAudioThreadStartLock;
    Condition *iAudioThreadStartCV;
    volatile status_t iAudioThreadStartResult;
    volatile bool iAudioThreadStarted;

    PVMFMediaClock *iAuthorClock;
    PVMFMediaClockNotificationsInterface *iClockNotificationsInf;

    // These variables tracks whether or not first audio frame was received.
    // This is needed to start the clock since the time origin is synced to the
    // first audio sample.
    volatile bool iFirstFrameReceived;
    volatile PVMFTimestamp iFirstFrameTs;

    // This stores the Start cmd when Audio MIO is waiting for
    // first audio frame to be received from the device.
    AndroidAudioInputCmd iStartCmd;
};

}; // namespace android

#ifdef HIDE_MIO_SYMBOLS
#pragma GCC visibility pop
#endif

#endif // ANDROID_AUDIO_INPUT_H_INCLUDED
