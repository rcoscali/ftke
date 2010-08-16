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
#ifndef PV_FRAME_METADATA_MIO_AUDIO_H_INCLUDED
#define PV_FRAME_METADATA_MIO_AUDIO_H_INCLUDED

#ifndef PVMI_MIO_CONTROL_H_INCLUDED
#include "pvmi_mio_control.h"
#endif
#ifndef PVMI_MEDIA_TRANSFER_H_INCLUDED
#include "pvmi_media_transfer.h"
#endif
#ifndef OSCL_SCHEDULER_AO_H_INCLUDED
#include "oscl_scheduler_ao.h"
#endif
#ifndef PVMI_MEDIA_IO_OBSERVER_H_INCLUDED
#include "pvmi_media_io_observer.h"
#endif
#ifndef PVMI_CONFIG_AND_CAPABILITY_H_INCLUDED
#include "pvmi_config_and_capability.h"
#endif
#ifndef OSCL_STRING_CONTAINERS_H_INCLUDED
#include "oscl_string_containers.h"
#endif
#ifndef PVMI_MEDIA_IO_CLOCK_EXTENSION_H_INCLUDED
#include "pvmi_media_io_clock_extension.h"
#endif

class PVLogger;
class PVMFMediaClock;
class ColorConvertBase;

class PVFMAudioMIOGetFrameObserver
{
    public:
        virtual void HandleFrameReadyEvent(PVMFStatus aEventStatus) = 0;
};

// Provide the clock interface so the MIO can do synchronization
class PVFMAudioMIOActiveTimingSupport: public PvmiClockExtensionInterface
{
    public:
        PVFMAudioMIOActiveTimingSupport() :
                iClock(NULL)
        {}

        virtual ~PVFMAudioMIOActiveTimingSupport()
        {}

        // From PvmiClockExtensionInterface
        PVMFStatus SetClock(PVMFMediaClock *aClock);

        // From PVInterface
        void addRef() ;
        void removeRef() ;
        bool queryInterface(const PVUuid& uuid, PVInterface*& iface) ;

        void queryUuid(PVUuid& uuid);

        PVMFMediaClock* iClock;
};


// This class implements the media IO component for extracting video frames
// for pvFrameAndMetadata utility.
// This class constitutes the Media IO component

class PVFMAudioMIO : public OsclTimerObject,
        public PvmiMIOControl,
        public PvmiMediaTransfer,
        public PvmiCapabilityAndConfig
{
    public:
        PVFMAudioMIO();
        ~PVFMAudioMIO();

        // Frame retrieval APIs
        PVMFStatus GetFrameByFrameNumber(uint32 aFrameIndex, uint8* aFrameBuffer, uint32& aBufferSize, PVMFFormatType aFormaType, PVFMAudioMIOGetFrameObserver& aObserver);
        PVMFStatus GetFrameByTimeoffset(uint32 aTimeOffset, uint8* aFrameBuffer, uint32& aBufferSize, PVMFFormatType aFormatType, PVFMAudioMIOGetFrameObserver& aObserver);
        PVMFStatus CancelGetFrame(void);
        PVMFStatus GetFrameProperties(uint32& aFrameWidth, uint32& aFrameHeight, uint32& aDisplayWidth, uint32& aDisplayHeight);

        // From PvmiMIOControl
        PVMFStatus connect(PvmiMIOSession& aSession, PvmiMIOObserver* aObserver);
        PVMFStatus disconnect(PvmiMIOSession aSession);
        PVMFCommandId QueryUUID(const PvmfMimeString& aMimeType, Oscl_Vector<PVUuid, OsclMemAllocator>& aUuids,
                                bool aExactUuidsOnly = false, const OsclAny* aContext = NULL);
        PVMFCommandId QueryInterface(const PVUuid& aUuid, PVInterface*& aInterfacePtr, const OsclAny* aContext = NULL);
        PvmiMediaTransfer* createMediaTransfer(PvmiMIOSession& aSession, PvmiKvp* read_formats = NULL, int32 read_flags = 0,
                                               PvmiKvp* write_formats = NULL, int32 write_flags = 0);
        void deleteMediaTransfer(PvmiMIOSession& aSession, PvmiMediaTransfer* media_transfer);
        PVMFCommandId Init(const OsclAny* aContext = NULL);
        PVMFCommandId Reset(const OsclAny* aContext = NULL);
        PVMFCommandId Start(const OsclAny* aContext = NULL);
        PVMFCommandId Pause(const OsclAny* aContext = NULL);
        PVMFCommandId Flush(const OsclAny* aContext = NULL);
        PVMFCommandId DiscardData(const OsclAny* aContext = NULL);
        PVMFCommandId DiscardData(PVMFTimestamp aTimestamp, const OsclAny* aContext = NULL);
        PVMFCommandId Stop(const OsclAny* aContext = NULL);
        PVMFCommandId CancelAllCommands(const OsclAny* aContext = NULL);
        PVMFCommandId CancelCommand(PVMFCommandId aCmdId, const OsclAny* aContext = NULL);
        void ThreadLogon();
        void ThreadLogoff();

        // From PvmiMediaTransfer
        void setPeer(PvmiMediaTransfer* aPeer);
        void useMemoryAllocators(OsclMemAllocator* write_alloc = NULL);
        PVMFCommandId writeAsync(uint8 format_type, int32 format_index, uint8* data, uint32 data_len,
                                 const PvmiMediaXferHeader& data_header_info, OsclAny* aContext = NULL);
        void writeComplete(PVMFStatus aStatus, PVMFCommandId write_cmd_id, OsclAny* aContext);
        PVMFCommandId readAsync(uint8* data, uint32 max_data_len, OsclAny* aContext = NULL,
                                int32* formats = NULL, uint16 num_formats = 0);
        void readComplete(PVMFStatus aStatus, PVMFCommandId  read_cmd_id, int32 format_index,
                          const PvmiMediaXferHeader& data_header_info, OsclAny* aContext);
        void statusUpdate(uint32 status_flags);
        void cancelCommand(PVMFCommandId  command_id);
        void cancelAllCommands();

        // From PvmiCapabilityAndConfig
        void setObserver(PvmiConfigAndCapabilityCmdObserver* aObserver);
        PVMFStatus getParametersSync(PvmiMIOSession aSession, PvmiKeyType aIdentifier, PvmiKvp*& aParameters,
                                     int& num_parameter_elements, PvmiCapabilityContext aContext);
        PVMFStatus releaseParameters(PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements);
        void createContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext);
        void setContextParameters(PvmiMIOSession aSession, PvmiCapabilityContext& aContext, PvmiKvp* aParameters,
                                  int num_parameter_elements);
        void DeleteContext(PvmiMIOSession aSession, PvmiCapabilityContext& aContext);
        void setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements, PvmiKvp*& aRet_kvp);
        PVMFCommandId setParametersAsync(PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements,
                                         PvmiKvp*& aRet_kvp, OsclAny* context = NULL);
        uint32 getCapabilityMetric(PvmiMIOSession aSession);
        PVMFStatus verifyParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters, int num_elements);

    private:
        void InitData();

        // From OsclTimerObject
        void Run();

        void Reschedule();

        void Cleanup();
        void ResetData();

        // Copy video frame data to provided including YUV to RGB conversion if necessary
        PVMFStatus CopyAudioFrameData(uint8* aSrcBuffer, uint32 aSrcSize, PVMFFormatType aSrcFormat,
                                      uint8* aDestBuffer, uint32& aDestSize, PVMFFormatType aDestFormat,
                                      uint32 iSrcWidth, uint32 iSrcHeight, uint32 iDestWidth, uint32 iDestHeight);
        PVMFStatus CreateYUVToRGBColorConverter(ColorConvertBase*& aCC, PVMFFormatType aRGBFormatType);
        PVMFStatus DestroyYUVToRGBColorConverter(ColorConvertBase*& aCC, PVMFFormatType aRGBFormatType);

        PvmiMediaTransfer* iPeer;

        // The PvmiMIOControl class observer.
        PvmiMIOObserver* iObserver;

        // For generating command IDs
        uint32 iCommandCounter;

        // State
        enum PVFMVMIOState
        {
            STATE_IDLE
            , STATE_LOGGED_ON
            , STATE_INITIALIZED
            , STATE_STARTED
            , STATE_PAUSED
        };
        PVFMVMIOState iState;

        // Control command handling.
        class CommandResponse
        {
            public:
                CommandResponse(PVMFStatus s, PVMFCommandId id, const OsclAny* ctx)
                        : iStatus(s), iCmdId(id), iContext(ctx)
                {}

                PVMFStatus iStatus;
                PVMFCommandId iCmdId;
                const OsclAny* iContext;
        };
        Oscl_Vector<CommandResponse, OsclMemAllocator> iCommandResponseQueue;
        void QueueCommandResponse(CommandResponse&);

        // Write command handling
        class WriteResponse
        {
            public:
                WriteResponse(PVMFStatus s, PVMFCommandId id, const OsclAny* ctx, const PVMFTimestamp& ts)
                        : iStatus(s), iCmdId(id), iContext(ctx), iTimestamp(ts)
                {}

                PVMFStatus iStatus;
                PVMFCommandId iCmdId;
                const OsclAny* iContext;
                PVMFTimestamp iTimestamp;
        };
        Oscl_Vector<WriteResponse, OsclMemAllocator> iWriteResponseQueue;

        // Audio parameters
        PVMFFormatType iAudioFormat;
        uint32 iAudioNumChannels;
        bool iAudioNumChannelsValid;
        uint32 iAudioSamplingRate;
        bool iAudioSamplingRateValid;
        bool iIsMIOConfigured;
        bool iWriteBusy;

        // For logging
        PVLogger* iLogger;

        Oscl_Vector<PVMFFormatType, OsclMemAllocator> iInputFormatCapability;

};

#endif // PV_FRAME_METADATA_MIO_AUDIO_H_INCLUDED

