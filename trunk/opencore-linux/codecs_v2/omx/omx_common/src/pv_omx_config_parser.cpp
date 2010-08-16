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

#include "oscl_stdstring.h"

// Use default DLL entry point
#ifndef OSCL_DLL_H_INCLUDED
#include "oscl_dll.h"
#endif

#include "pv_omx_config_parser.h"
#include "pv_omxcore.h"

#if (USE_DYNAMIC_LOAD_OMX_COMPONENTS == 0)
// in case of static build - just redirect master omx core call to local pv core call
OSCL_EXPORT_REF OMX_BOOL OMX_MasterConfigParser(
    OMX_PTR aInputParameters,
    OMX_PTR aOutputParameters)

{
    return OMXConfigParser(aInputParameters, aOutputParameters);
}
#endif

OSCL_EXPORT_REF OMX_BOOL OMXConfigParser(
    OMX_PTR aInputParameters,
    OMX_PTR aOutputParameters)

{
    OMXConfigParserInputs* pInputs;

    pInputs = (OMXConfigParserInputs*) aInputParameters;


    if (NULL != pInputs->cComponentRole)
    {
        if (0 == oscl_strncmp(pInputs->cComponentRole, (OMX_STRING)"audio_decoder", oscl_strlen("audio_decoder")))
        {
            OMX_S32 Status;
            pvAudioConfigParserInputs aInputs;

            aInputs.inPtr = pInputs->inPtr;
            aInputs.inBytes = pInputs->inBytes;

            if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"audio_decoder.wma"))
            {
                aInputs.iMimeType = PVMF_MIME_WMA;

            }
            else if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"audio_decoder.aac"))
            {
                aInputs.iMimeType = PVMF_MIME_AAC_SIZEHDR;

            }
            else if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"audio_decoder.amr"))
            {
                aInputs.iMimeType = PVMF_MIME_AMR;

            }
            else if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"audio_decoder.amrnb"))
            {
                aInputs.iMimeType = PVMF_MIME_AMR;

            }
            else if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"audio_decoder.amrwb"))
            {
                aInputs.iMimeType = PVMF_MIME_AMRWB;

            }
            else if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"audio_decoder.mp3"))
            {
                aInputs.iMimeType = PVMF_MIME_MP3;

            }
            else
            {
                return OMX_FALSE;
            }


            Status = pv_audio_config_parser(&aInputs, (pvAudioConfigParserOutputs *)aOutputParameters);
            if (0 == Status)
            {
                return OMX_FALSE;
            }
        }
        else if (0 == oscl_strncmp(pInputs->cComponentRole, (OMX_STRING)"video_decoder", oscl_strlen("video_decoder")))
        {

            OMX_S32 Status;
            pvVideoConfigParserInputs aInputs;

            aInputs.inPtr = pInputs->inPtr;
            aInputs.inBytes = pInputs->inBytes;

            if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"video_decoder.wmv"))
            {
                aInputs.iMimeType = PVMF_MIME_WMV;

            }
            else if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"video_decoder.avc"))
            {
                aInputs.iMimeType = PVMF_MIME_H264_VIDEO;

            }
            else if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"video_decoder.mpeg4"))
            {
                aInputs.iMimeType = PVMF_MIME_M4V;

            }
            else if (0 == oscl_strcmp(pInputs->cComponentRole, (OMX_STRING)"video_decoder.h263"))
            {
                aInputs.iMimeType = PVMF_MIME_H2632000;

            }
            else
            {
                return OMX_FALSE;
            }

            Status = pv_video_config_parser(&aInputs, (pvVideoConfigParserOutputs *)aOutputParameters);
            if (0 != Status)
            {
                return OMX_FALSE;
            }
        }
        else
        {
            return OMX_FALSE;
        }

    }
    else
    {
        return OMX_FALSE;
    }

    return OMX_TRUE;
}

