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
#ifndef PVMF_FILEOUTPUT_NODE_H_INCLUDED
#include "pvmf_fileoutput_node.h"
#endif

#ifndef OSCL_MIME_STRING_UTILS_H
#include "pv_mime_string_utils.h"
#endif

#ifndef PVMI_KVP_UTIL_H_INCLUDED
#include "pvmi_kvp_util.h"
#endif

struct FileOutputNodeKeyStringData
{
    char iString[64];
    PvmiKvpType iType;
    PvmiKvpValueType iValueType;
};

#define FILEOUTPUTNODECONFIG_BASE_NUMKEYS 2
#define FILEOUTPUTCONFIG_KEYSTRING_SIZE 128

static const FileOutputNodeKeyStringData FileOutputNodeConfig_BaseKeys[] =
{
    {"parameter1", PVMI_KVPTYPE_VALUE, PVMI_KVPVALTYPE_UINT32},
    {"parameter2", PVMI_KVPTYPE_VALUE, PVMI_KVPVALTYPE_UINT32}
};

enum BaseKeys_IndexMapType
{
    PARAMETER1 = 0,
    PARAMETER2
};
static const uint FileOutputNodeConfig_NumBaseKeys =
    (sizeof(FileOutputNodeConfig_BaseKeys) /
     sizeof(FileOutputNodeKeyStringData));


///////////////////////////////////////////////////////////////////////////
PVMFStatus PVMFFileOutputNode::GetConfigParameter(PvmiKvp*& aParameters, int& aNumParamElements, int32 aIndex, PvmiKvpAttr aReqattr)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::GetConfigParameter() In"));

    aNumParamElements = 0;

    // Allocate memory for the KVP
    aParameters = (PvmiKvp*)oscl_malloc(sizeof(PvmiKvp));
    if (aParameters == NULL)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::GetConfigParameter() Memory allocation for KVP failed"));
        return PVMFErrNoMemory;
    }
    oscl_memset(aParameters, 0, sizeof(PvmiKvp));
    // Allocate memory for the key string in KVP
    PvmiKeyType memblock = (PvmiKeyType)oscl_malloc(FILEOUTPUTCONFIG_KEYSTRING_SIZE * sizeof(char));
    if (NULL == memblock)
    {
        oscl_free(aParameters);
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::GetConfigParameter() Memory allocation for key string failed"));
        return PVMFErrNoMemory;
    }
    oscl_strset(memblock, 0, FILEOUTPUTCONFIG_KEYSTRING_SIZE * sizeof(char));
    // Assign the key string buffer to KVP
    aParameters[0].key = memblock;

    // Copy the key string
    oscl_strncat(aParameters[0].key, _STRLIT_CHAR("x-pvmf/file/output/"), 21);
    oscl_strncat(aParameters[0].key, FileOutputNodeConfig_BaseKeys[aIndex].iString, oscl_strlen(FileOutputNodeConfig_BaseKeys[aIndex].iString));
    oscl_strncat(aParameters[0].key, _STRLIT_CHAR(";type=value;valtype="), 20);
    switch (FileOutputNodeConfig_BaseKeys[aIndex].iValueType)
    {
        case PVMI_KVPVALTYPE_BITARRAY32:
            oscl_strncat(aParameters[0].key, _STRLIT_CHAR(PVMI_KVPVALTYPE_BITARRAY32_STRING), oscl_strlen(PVMI_KVPVALTYPE_BITARRAY32_STRING));
            break;

        case PVMI_KVPVALTYPE_KSV:
            oscl_strncat(aParameters[0].key, _STRLIT_CHAR(PVMI_KVPVALTYPE_KSV_STRING), oscl_strlen(PVMI_KVPVALTYPE_KSV_STRING));
            break;

        case PVMI_KVPVALTYPE_BOOL:
            oscl_strncat(aParameters[0].key, _STRLIT_CHAR(PVMI_KVPVALTYPE_BOOL_STRING), oscl_strlen(PVMI_KVPVALTYPE_BOOL_STRING));
            break;

        case PVMI_KVPVALTYPE_INT32:
            if (aReqattr == PVMI_KVPATTR_CUR)
            {
                oscl_strncat(aParameters[0].key, _STRLIT_CHAR(PVMI_KVPVALTYPE_INT32_STRING), oscl_strlen(PVMI_KVPVALTYPE_RANGE_UINT32_STRING));
            }
            break;
        case PVMI_KVPVALTYPE_UINT32:
        default:
            if (PVMI_KVPATTR_CAP == aReqattr)
            {
                oscl_strncat(aParameters[0].key, _STRLIT_CHAR(PVMI_KVPVALTYPE_RANGE_UINT32_STRING), oscl_strlen(PVMI_KVPVALTYPE_RANGE_UINT32_STRING));
            }
            else
            {
                oscl_strncat(aParameters[0].key, _STRLIT_CHAR(PVMI_KVPVALTYPE_UINT32_STRING), oscl_strlen(PVMI_KVPVALTYPE_UINT32_STRING));
            }
            break;
    }
    aParameters[0].key[FILEOUTPUTCONFIG_KEYSTRING_SIZE-1] = 0;

    // Copy the requested info
    switch (aIndex)
    {
        case PARAMETER1:    // "parameter1"
            if (PVMI_KVPATTR_CUR == aReqattr)
            {
                // Return current value
                // get the parameter value here
            }
            else if (PVMI_KVPATTR_DEF == aReqattr)
            {
                // return default
            }
            else
            {
                // Return capability
            }
            break;

        case PARAMETER2:    // "parameter2"
            if (PVMI_KVPATTR_CUR == aReqattr)
            {
                // Return current value
                // get the parameter value here

            }
            else if (PVMI_KVPATTR_DEF == aReqattr)
            {
                // return default
            }
            else
            {
                // Return capability
            }
            break;

        default:
            // Invalid index
            oscl_free(aParameters[0].key);
            oscl_free(aParameters);
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::GetConfigParameter() Invalid index to file output node parameter"));
            return PVMFErrNotSupported;
    }

    aNumParamElements = 1;

    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::GetConfigParameter() Out"));
    return PVMFSuccess;
}


PVMFStatus PVMFFileOutputNode::VerifyAndSetConfigParameter(PvmiKvp& aParameter, bool aSetParam)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::VerifyAndSetConfigParameter() In"));

    // Determine the valtype
    PvmiKvpValueType keyvaltype = GetValTypeFromKeyString(aParameter.key);
    if (keyvaltype == PVMI_KVPVALTYPE_UNKNOWN)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::VerifyAndSetConfigParameter() Valtype in key string unknown"));
        return PVMFErrNotSupported;
    }

    // Retrieve the fourth component from the key string
    char* compstr = NULL;
    pv_mime_string_extract_type(3, aParameter.key, compstr);

    int32 fileoutput4ind;
    for (fileoutput4ind = 0; fileoutput4ind < FILEOUTPUTNODECONFIG_BASE_NUMKEYS; ++fileoutput4ind)
    {
        // Go through each component string at 4th level
        if (pv_mime_strcmp(compstr, (char*)(FileOutputNodeConfig_BaseKeys[fileoutput4ind].iString)) >= 0)
        {
            // Break out of the for loop
            break;
        }
    }

    if (FILEOUTPUTNODECONFIG_BASE_NUMKEYS <= fileoutput4ind)
    {
        // Match couldn't be found or non-leaf node specified
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::VerifyAndSetConfigParameter() Unsupported key or non-leaf node"));
        return PVMFErrNotSupported;
    }

    // Verify the valtype
    if (keyvaltype != FileOutputNodeConfig_BaseKeys[fileoutput4ind].iValueType)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::VerifyAndSetConfigParameter() Valtype does not match for key"));
        return PVMFErrNotSupported;
    }

    switch (fileoutput4ind)
    {
        case PARAMETER1: // "parameter1"
            // Change the parameter
            if (aSetParam)
            {
                // set the parameter here
            }
            break;

        case PARAMETER2: // "parameter2"
            // change the parameter
            if (aSetParam)
            {
                // set any parameter here
            }
            break;

        default:
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::VerifyAndSetConfigParameter() Invalid index for file output node parameter"));
            return PVMFErrNotSupported;
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::VerifyAndSetConfigParameter() Out"));
    return PVMFSuccess;
}

////////////////////////////////////////////////////////////////////////////
//                  PvmiCapConfigInterface Virtual Functions
////////////////////////////////////////////////////////////////////////////

void PVMFFileOutputNode::createContext(PvmiMIOSession aSession,
                                       PvmiCapabilityContext& aContext)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);
    // not supported
    OSCL_LEAVE(PVMFErrNotSupported);
}

void PVMFFileOutputNode::setContextParameters(PvmiMIOSession aSession,
        PvmiCapabilityContext& aContext,
        PvmiKvp* aParameters,
        int aNumElements)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);
    OSCL_UNUSED_ARG(aParameters);
    OSCL_UNUSED_ARG(aNumElements);
    // not supported
    OSCL_LEAVE(PVMFErrNotSupported);
}

void PVMFFileOutputNode::DeleteContext(PvmiMIOSession aSession,
                                       PvmiCapabilityContext& aContext)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);
    // not supported
    OSCL_LEAVE(PVMFErrNotSupported);
}

PVMFCommandId PVMFFileOutputNode::setParametersAsync(PvmiMIOSession aSession,
        PvmiKvp* aParameters,
        int aNumElements,
        PvmiKvp*& aRet_kvp,
        OsclAny* aContext)
{
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);
    OSCL_UNUSED_ARG(aParameters);
    OSCL_UNUSED_ARG(aRet_kvp);
    OSCL_UNUSED_ARG(aNumElements);
    return 0;
}

uint32 PVMFFileOutputNode::getCapabilityMetric(PvmiMIOSession aSession)
{
    OSCL_UNUSED_ARG(aSession);
    return 0;
}

PVMFStatus PVMFFileOutputNode::verifyParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters, int aNumElements)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::verifyParametersSync()"));
    OSCL_UNUSED_ARG(aSession);

    if (NULL == aParameters || aNumElements < 1)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::verifyParametersSync() Passed in parameter invalid"));
        return PVMFErrArgument;
    }

    // Go through each parameter
    for (int32 paramind = 0; paramind < aNumElements; ++paramind)
    {
        // Count the number of components and parameters in the key
        int compcount = pv_mime_string_compcnt(aParameters[paramind].key);
        // Retrieve the first component from the key string
        char* compstr = NULL;
        pv_mime_string_extract_type(0, aParameters[paramind].key, compstr);

        if ((pv_mime_strcmp(compstr, _STRLIT_CHAR("x-pvmf/file/output")) < 0) || compcount < 4)
        {
            // First 4 components should be "x-pvmf/file/output" and there must
            // be at least four components
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::verifyParametersSync() Unsupported key"));
            return PVMFErrNotSupported;
        }

        if (4 == compcount)
        {
            // Verify and set the passed-in file output node setting
            PVMFStatus retval = VerifyAndSetConfigParameter(aParameters[paramind], false);
            if (retval != PVMFSuccess)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::verifyParametersSync() Setting parameter %d failed", paramind));
                return retval;
            }
        }
        else
        {
            // Do not support more than 4 components right now
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::verifyParametersSync() Unsupported key"));
            return PVMFErrNotSupported;
        }
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::verifyParametersSync() Out"));
    return PVMFSuccess;
}

PVMFStatus PVMFFileOutputNode::releaseParameters(PvmiMIOSession aSession, PvmiKvp* aParameters, int aNumElements)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::releaseParameters()"));
    OSCL_UNUSED_ARG(aSession);

    if (aParameters == NULL || aNumElements < 1)
    {
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::releaseParameters() KVP list is NULL or number of elements is 0"));
        return PVMFErrArgument;
    }

    // Count the number of components and parameters in the key
    int compcount = pv_mime_string_compcnt(aParameters[0].key);
    // Retrieve the first component from the key string
    char* compstr = NULL;
    pv_mime_string_extract_type(0, aParameters[0].key, compstr);

    if ((pv_mime_strcmp(compstr, _STRLIT_CHAR("x-pvmf/file/output")) < 0) || compcount < 3)
    {
        // First 3 component should be "x-pvmf/file/output" and there must
        // be at least four components
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::releaseParameters() Unsupported key"));
        return PVMFErrNotSupported;
    }

    // Retrieve the third component from the key string
    pv_mime_string_extract_type(3, aParameters[0].key, compstr);

    // Go through each KVP and release memory for value if allocated from heap
    for (int32 ii = 0; ii < aNumElements; ++ii)
    {
        // Next check if it is a value type that allocated memory
        PvmiKvpType kvptype = GetTypeFromKeyString(aParameters[ii].key);
        if (kvptype == PVMI_KVPTYPE_VALUE || kvptype == PVMI_KVPTYPE_UNKNOWN)
        {
            PvmiKvpValueType keyvaltype = GetValTypeFromKeyString(aParameters[ii].key);
            if (PVMI_KVPVALTYPE_UNKNOWN == keyvaltype)
            {
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::releaseParameters() Valtype not specified in key string"));
                return PVMFErrNotSupported;
            }

            if (((PVMI_KVPVALTYPE_CHARPTR == keyvaltype) && NULL != (aParameters[ii].value.pChar_value)))
            {
                oscl_free(aParameters[ii].value.pChar_value);
                aParameters[ii].value.pChar_value = NULL;
            }
            else if (PVMI_KVPVALTYPE_KSV == keyvaltype && NULL != aParameters[ii].value.key_specific_value)
            {
                oscl_free(aParameters[ii].value.key_specific_value);
                aParameters[ii].value.key_specific_value = NULL;
            }
            else if (PVMI_KVPVALTYPE_RANGE_UINT32 == keyvaltype && NULL != aParameters[ii].value.key_specific_value)
            {
                range_uint32* rui32 = (range_uint32*)aParameters[ii].value.key_specific_value;
                aParameters[ii].value.key_specific_value = NULL;
                oscl_free(rui32);
            }
            // @TODO Add more types if file output node starts returning more types
        }
    }

    // file output node allocated its key strings in one chunk so just free the first key string ptr
    oscl_free(aParameters[0].key);

    // Free memory for the parameter list
    oscl_free(aParameters);
    aParameters = NULL;

    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::releaseParameters() Out"));
    return PVMFSuccess;
}



PVMFStatus PVMFFileOutputNode::getParametersSync(PvmiMIOSession aSession, PvmiKeyType aIdentifier, PvmiKvp*& aParameters, int& aNumParamElements, PvmiCapabilityContext aContext)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::getParametersSync()"));
    OSCL_UNUSED_ARG(aSession);
    OSCL_UNUSED_ARG(aContext);

    // Initialize the output parameters
    aNumParamElements = 0;
    aParameters = NULL;

    // Count the number of components and parameters in the key
    int compcount = pv_mime_string_compcnt(aIdentifier);
    // Retrieve the first component from the key string
    char* compstr = NULL;
    pv_mime_string_extract_type(0, aIdentifier, compstr);

    if ((pv_mime_strcmp(compstr, _STRLIT_CHAR("x-pvmf/file/output")) < 0) || compcount < 4)
    {
        // First 4 components should be "x-pvmf/file/output" and there must
        // be at least 4 components
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::getParametersSync() Invalid key string"));
        return PVMFErrNotSupported;
    }
    // Retrieve the fourth component from the key string
    pv_mime_string_extract_type(3, aIdentifier, compstr);

    for (int32 fileoutput4ind = 0; fileoutput4ind < FILEOUTPUTNODECONFIG_BASE_NUMKEYS; ++fileoutput4ind)
    {
        // Go through each file output  component string at 4th level
        if (pv_mime_strcmp(compstr, (char*)(FileOutputNodeConfig_BaseKeys[fileoutput4ind].iString)) >= 0)
        {
            if (4 == compcount)
            {
                // Determine what is requested
                PvmiKvpAttr reqattr = GetAttrTypeFromKeyString(aIdentifier);
                if (reqattr == PVMI_KVPATTR_UNKNOWN)
                {
                    reqattr = PVMI_KVPATTR_CUR;
                }

                // Return the requested info
                PVMFStatus retval = GetConfigParameter(aParameters, aNumParamElements, fileoutput4ind, reqattr);
                if (retval != PVMFSuccess)
                {
                    PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::getParametersSync() Retrieving file output node parameter failed"));
                    return retval;
                }
            }
            else
            {
                // Right now file output node doesn't support more than 4 components
                // for this sub-key string so error out
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::getParametersSync() Unsupported key"));
                return PVMFErrNotSupported;
            }
        }
    }

    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::getParametersSync() Out"));
    if (0 == aNumParamElements)
    {
        // If no one could get the parameter, return error
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::getParametersSync() Unsupported key"));
        return PVMFFailure;
    }
    else
    {
        return PVMFSuccess;
    }
}

void PVMFFileOutputNode::setParametersSync(PvmiMIOSession aSession, PvmiKvp* aParameters, int aNumElements, PvmiKvp* &aRetKVP)
{
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::setParametersSync()"));
    OSCL_UNUSED_ARG(aSession);
    // Complete the request synchronously

    if (NULL == aParameters || aNumElements < 1)
    {
        if (aParameters)
        {
            aRetKVP = aParameters;
        }
        PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::setParametersSync() Passed in parameter invalid"));
        return;
    }

    // Go through each parameter
    for (int32 paramind = 0; paramind < aNumElements; ++paramind)
    {
        // Count the number of components and parameters in the key
        int compcount = pv_mime_string_compcnt(aParameters[paramind].key);
        // Retrieve the first component from the key string
        char* compstr = NULL;
        pv_mime_string_extract_type(0, aParameters[paramind].key, compstr);

        if ((pv_mime_strcmp(compstr, _STRLIT_CHAR("x-pvmf/file/output")) < 0) || compcount < 4)
        {
            // First 3 components should be "x-pvmf/file/output" and there must
            // be at least four components
            aRetKVP = &aParameters[paramind];
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::setParametersSync() Unsupported key"));
            return;
        }

        if (4 == compcount)
        {
            // Verify and set the passed-in mp4 file output node setting
            PVMFStatus retval = VerifyAndSetConfigParameter(aParameters[paramind], true);
            if (PVMFSuccess != retval)
            {
                aRetKVP = &aParameters[paramind];
                PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::setParametersSync() Setting parameter %d failed", paramind));
                return;
            }
        }
        else
        {
            // Do not support more than 5 components right now
            aRetKVP = &aParameters[paramind];
            PVLOGGER_LOGMSG(PVLOGMSG_INST_HLDBG, iLogger, PVLOGMSG_ERR, (0, "PVMFFileOutputNode::setParametersSync() Unsupported key"));
            return;
        }
    }
    PVLOGGER_LOGMSG(PVLOGMSG_INST_LLDBG, iLogger, PVLOGMSG_STACK_TRACE, (0, "PVMFFileOutputNode::setParametersSync() Out"));
}


void PVMFFileOutputNode::setObserver(PvmiConfigAndCapabilityCmdObserver* aObserver)
{
    ciObserver = aObserver;
}
