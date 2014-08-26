/**************************************************************************
 *
 * Copyright 2012 BMW Car IT GmbH
 * Copyright (C) 2012 DENSO CORPORATION and Robert Bosch Car Multimedia Gmbh
 * Copyright (C) 2012 Bayerische Motorenwerke Aktiengesellschaft
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ****************************************************************************/
#include "ilm_common.h"

ilmErrorTypes ilm_getPropertiesOfLayer(t_ilm_uint layerID, struct ilmLayerProperties* pLayerProperties)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetPropertiesOfLayer");
    if (pLayerProperties
        && command
        && gIpcModule.appendUint(command, layerID)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getDouble(response, &pLayerProperties->opacity)
        && gIpcModule.getUint(response, &pLayerProperties->sourceX)
        && gIpcModule.getUint(response, &pLayerProperties->sourceY)
        && gIpcModule.getUint(response, &pLayerProperties->sourceWidth)
        && gIpcModule.getUint(response, &pLayerProperties->sourceHeight)
        && gIpcModule.getUint(response, &pLayerProperties->origSourceWidth)
        && gIpcModule.getUint(response, &pLayerProperties->origSourceHeight)
        && gIpcModule.getUint(response, &pLayerProperties->destX)
        && gIpcModule.getUint(response, &pLayerProperties->destY)
        && gIpcModule.getUint(response, &pLayerProperties->destWidth)
        && gIpcModule.getUint(response, &pLayerProperties->destHeight)
        && gIpcModule.getUint(response, &pLayerProperties->orientation)
        && gIpcModule.getBool(response, &pLayerProperties->visibility)
        && gIpcModule.getUint(response, &pLayerProperties->type)
        && gIpcModule.getBool(response, &pLayerProperties->chromaKeyEnabled)
        && gIpcModule.getUint(response, &pLayerProperties->chromaKeyRed)
        && gIpcModule.getUint(response, &pLayerProperties->chromaKeyGreen)
        && gIpcModule.getUint(response, &pLayerProperties->chromaKeyBlue)
        && gIpcModule.getInt(response, &pLayerProperties->creatorPid))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_getPropertiesOfScreen(t_ilm_display screenID, struct ilmScreenProperties* pScreenProperties)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    if (pScreenProperties)
    {
        t_ilm_message response = 0;
        t_ilm_message command = gIpcModule.createMessage("GetPropertiesOfScreen");
        if (command
            && gIpcModule.appendUint(command, screenID)
            && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
            && gIpcModule.getUintArray(response, &pScreenProperties->layerIds, (int*)(&pScreenProperties->layerCount))
            && gIpcModule.getUint(response, &pScreenProperties->harwareLayerCount)
            && gIpcModule.getUint(response, &pScreenProperties->screenWidth)
            && gIpcModule.getUint(response, &pScreenProperties->screenHeight))
        {
            returnValue = ILM_SUCCESS;
        }
        else
        {
            pScreenProperties->layerCount = 0;
            pScreenProperties->harwareLayerCount = 0;
            pScreenProperties->layerIds = NULL;
            pScreenProperties->screenWidth = 0;
            pScreenProperties->screenHeight = 0;
        }

        gIpcModule.destroyMessage(response);
        gIpcModule.destroyMessage(command);
    }
    return returnValue;
}

ilmErrorTypes ilm_getNumberOfHardwareLayers(t_ilm_uint screenID, t_ilm_uint* pNumberOfHardwareLayers)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetNumberOfHardwareLayers");
    if (pNumberOfHardwareLayers
        && command
        && gIpcModule.appendUint(command, screenID)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pNumberOfHardwareLayers))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_getScreenIDs(t_ilm_uint* pNumberOfIDs, t_ilm_uint** ppIDs)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetScreenIDs");
    if (pNumberOfIDs && ppIDs
        && command
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUintArray(response, ppIDs, (t_ilm_int *)pNumberOfIDs))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_getLayerIDs(t_ilm_int* pLength, t_ilm_layer** ppArray)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("ListAllLayerIDS");
    if (pLength && ppArray
        && command
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUintArray(response, ppArray, pLength))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_getLayerIDsOnScreen(t_ilm_uint screenId, t_ilm_int* pLength, t_ilm_layer** ppArray)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("ListAllLayerIDsOnScreen");
    if (pLength && ppArray
        && command
        && gIpcModule.appendUint(command, screenId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUintArray(response, ppArray, pLength))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_getSurfaceIDs(t_ilm_int* pLength, t_ilm_surface** ppArray)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("ListAllSurfaceIDS");
    if (pLength && ppArray
        && command
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUintArray(response, ppArray, pLength))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_getSurfaceIDsOnLayer(t_ilm_layer layer, t_ilm_int* pLength, t_ilm_surface** ppArray)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("ListSurfaceofLayer");
    if (pLength && ppArray
        && command
        && gIpcModule.appendUint(command, layer)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUintArray(response, ppArray, pLength))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerCreateWithDimension(t_ilm_layer* pLayerId, t_ilm_uint width, t_ilm_uint height)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    if (pLayerId && (INVALID_ID != *pLayerId))
    {
        t_ilm_message response = 0;
        t_ilm_message command = gIpcModule.createMessage("CreateLayerFromIdWithDimension");
        if (command
            && gIpcModule.appendUint(command, *pLayerId)
            && gIpcModule.appendUint(command, width)
            && gIpcModule.appendUint(command, height)
            && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
            && gIpcModule.getUint(response, pLayerId))
        {
            returnValue = ILM_SUCCESS;
        }
        gIpcModule.destroyMessage(response);
        gIpcModule.destroyMessage(command);
    }
    else
    {
        t_ilm_message response = 0;
        t_ilm_message command = gIpcModule.createMessage("CreateLayerWithDimension");
        if (command
            && gIpcModule.appendUint(command, width)
            && gIpcModule.appendUint(command, height)
            && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
            && gIpcModule.getUint(response, pLayerId))
        {
            returnValue = ILM_SUCCESS;
        }
        gIpcModule.destroyMessage(response);
        gIpcModule.destroyMessage(command);
    }
    return returnValue;
}

ilmErrorTypes ilm_layerRemove(t_ilm_layer layerId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("RemoveLayer");
    if (command
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerGetType(t_ilm_layer layerId, ilmLayerType* pLayerType)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetLayerType");
    if (pLayerType
        && command
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pLayerType))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetVisibility(t_ilm_layer layerId, t_ilm_bool newVisibility)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetLayerVisibility");
    if (command
        && gIpcModule.appendUint(command, layerId)
        && gIpcModule.appendBool(command, newVisibility)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerGetVisibility(t_ilm_layer layerId, t_ilm_bool *pVisibility)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetLayerVisibility");
    if (pVisibility
        && command
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getBool(response, pVisibility))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetOpacity(t_ilm_layer layerId, t_ilm_float opacity)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetLayerOpacity");
    if (command
        && gIpcModule.appendUint(command, layerId)
        && gIpcModule.appendDouble(command, opacity)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerGetOpacity(t_ilm_layer layerId, t_ilm_float *pOpacity)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetLayerOpacity");
    if (pOpacity
        && command
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getDouble(response, pOpacity))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetSourceRectangle(t_ilm_layer layerId, t_ilm_uint x, t_ilm_uint y, t_ilm_uint width, t_ilm_uint height)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetLayerSourceRegion");
    if (command
        && gIpcModule.appendUint(command, layerId)
        && gIpcModule.appendUint(command, x)
        && gIpcModule.appendUint(command, y)
        && gIpcModule.appendUint(command, width)
        && gIpcModule.appendUint(command, height)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetDestinationRectangle(t_ilm_layer layerId, t_ilm_int x, t_ilm_int y, t_ilm_int width, t_ilm_int height)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetLayerDestinationRegion");
    if (command
        && gIpcModule.appendUint(command, layerId)
        && gIpcModule.appendUint(command, x)
        && gIpcModule.appendUint(command, y)
        && gIpcModule.appendUint(command, width)
        && gIpcModule.appendUint(command, height)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerGetDimension(t_ilm_layer layerId, t_ilm_uint *pDimension)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetLayerDimension");
    if (pDimension
        && command
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, &pDimension[0])
        && gIpcModule.getUint(response, &pDimension[1]))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetDimension(t_ilm_layer layerId, t_ilm_uint *pDimension)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetLayerDimension");
    if (pDimension
        && command
        && gIpcModule.appendUint(command, layerId)
        && gIpcModule.appendUint(command, pDimension[0])
        && gIpcModule.appendUint(command, pDimension[1])
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerGetPosition(t_ilm_layer layerId, t_ilm_uint *pPosition)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetLayerPosition");
    if (pPosition
        && command
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, &pPosition[0])
        && gIpcModule.getUint(response, &pPosition[1]))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetPosition(t_ilm_layer layerId, t_ilm_uint *pPosition)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetLayerPosition");
    if (pPosition
        && command
        && gIpcModule.appendUint(command, layerId)
        && gIpcModule.appendUint(command, pPosition[0])
        && gIpcModule.appendUint(command, pPosition[1])
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetOrientation(t_ilm_layer layerId, ilmOrientation orientation)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetLayerOrientation");
    if (command
        && gIpcModule.appendUint(command, layerId)
        && gIpcModule.appendUint(command, orientation)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerGetOrientation(t_ilm_layer layerId, ilmOrientation *pOrientation)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetLayerOrientation");
    if (pOrientation
        && command
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pOrientation))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetChromaKey(t_ilm_layer layerId, t_ilm_int* pColor)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetLayerChromaKey");
    if (command
        && gIpcModule.appendUint(command, layerId))
    {
        t_ilm_bool comResult = ILM_TRUE;

        /* Checking pColor has a content, otherwise chromakey is disabled */
        if (pColor)
        {
            const t_ilm_uint number = 3;
            comResult = gIpcModule.appendUintArray(command, (t_ilm_uint *)pColor, number);
        }
        if (comResult
            && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
        {
            returnValue = ILM_SUCCESS;
        }
        gIpcModule.destroyMessage(response);
    }
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerSetRenderOrder(t_ilm_layer layerId, t_ilm_layer *pSurfaceId, t_ilm_int number)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceRenderOrderWithinLayer");
    if (pSurfaceId
        && command
        && gIpcModule.appendUint(command, layerId)
        && gIpcModule.appendUintArray(command, pSurfaceId, number)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerGetCapabilities(t_ilm_layer layerId, t_ilm_layercapabilities *pCapabilities)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetLayerCapabilities");
    if (pCapabilities
        && command
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pCapabilities))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerTypeGetCapabilities(ilmLayerType layerType, t_ilm_layercapabilities *pCapabilities)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetLayertypeCapabilities");
    if (pCapabilities
        && command
        && gIpcModule.appendUint(command, layerType)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pCapabilities))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceInitialize(t_ilm_surface *pSurfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    if (pSurfaceId && (INVALID_ID != *pSurfaceId))
    {
        t_ilm_message response = 0;
        t_ilm_message command = gIpcModule.createMessage("InitializeSurfaceFromId");
        if (command
            && gIpcModule.appendUint(command, *pSurfaceId)
            && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
            && gIpcModule.getUint(response, pSurfaceId))
        {
            returnValue = ILM_SUCCESS;
        }
        gIpcModule.destroyMessage(response);
        gIpcModule.destroyMessage(command);
    }
    else
    {
        t_ilm_message response = 0;
        t_ilm_message command = gIpcModule.createMessage("InitializeSurface");
        if (command
            && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
            && gIpcModule.getUint(response, pSurfaceId))
        {
            returnValue = ILM_SUCCESS;
        }
        gIpcModule.destroyMessage(response);
        gIpcModule.destroyMessage(command);
    }
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetVisibility(t_ilm_surface surfaceId, t_ilm_bool newVisibility)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceVisibility");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendBool(command, newVisibility)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetOpacity(t_ilm_surface surfaceId, t_ilm_float opacity)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceOpacity");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendDouble(command, opacity)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceGetOpacity(t_ilm_surface surfaceId, t_ilm_float *pOpacity)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetSurfaceOpacity");
    if (pOpacity
        && command
        && gIpcModule.appendUint(command, surfaceId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getDouble(response, pOpacity))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_SetKeyboardFocusOn(t_ilm_surface surfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetKeyboardFocusOn");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_GetKeyboardFocusSurfaceId(t_ilm_surface* pSurfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetKeyboardFocusSurfaceId");
    if (command
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pSurfaceId))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetDestinationRectangle(t_ilm_surface surfaceId, t_ilm_int x, t_ilm_int y, t_ilm_int width, t_ilm_int height)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceDestinationRegion");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendUint(command, x)
        && gIpcModule.appendUint(command, y)
        && gIpcModule.appendUint(command, width)
        && gIpcModule.appendUint(command, height)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetDimension(t_ilm_surface surfaceId, t_ilm_uint *pDimension)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceDimension");
    if (pDimension
        && command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendUint(command, pDimension[0])
        && gIpcModule.appendUint(command, pDimension[1])
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceGetPosition(t_ilm_surface surfaceId, t_ilm_uint *pPosition)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetSurfacePosition");
    if (pPosition
        && command
        && gIpcModule.appendUint(command, surfaceId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, &pPosition[0])
        && gIpcModule.getUint(response, &pPosition[1]))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetPosition(t_ilm_surface surfaceId, t_ilm_uint *pPosition)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfacePosition");
    if (pPosition
        && command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendUint(command, pPosition[0])
        && gIpcModule.appendUint(command, pPosition[1])
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetOrientation(t_ilm_surface surfaceId, ilmOrientation orientation)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceOrientation");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendUint(command, orientation)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceGetOrientation(t_ilm_surface surfaceId, ilmOrientation *pOrientation)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetSurfaceOrientation");
    if (pOrientation
        && command
        && gIpcModule.appendUint(command, surfaceId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pOrientation))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceGetPixelformat(t_ilm_layer surfaceId, ilmPixelFormat *pPixelformat)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetSurfacePixelformat");
    if (pPixelformat
        && command
        && gIpcModule.appendUint(command, surfaceId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pPixelformat))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetChromaKey(t_ilm_surface surfaceId, t_ilm_int* pColor)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceChromaKey");
    if (command
        && gIpcModule.appendUint(command, surfaceId))
    {
        t_ilm_bool comResult = ILM_TRUE;

        /* Checking pColor has a content, otherwise chromakey is disabled */
        if (pColor)
        {
            const t_ilm_uint number = 3;
            comResult = gIpcModule.appendUintArray(command, (t_ilm_uint *)pColor, number);
        }
        if (comResult
            && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
        {
            returnValue = ILM_SUCCESS;
        }
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_displaySetRenderOrder(t_ilm_display display, t_ilm_layer *pLayerId, const t_ilm_uint number)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetRenderOrderOfLayers");
    if (pLayerId
        && command
        && gIpcModule.appendUintArray(command, pLayerId, number)
        && gIpcModule.appendUint(command, display)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_takeScreenshot(t_ilm_uint screen, t_ilm_const_string filename)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("ScreenShot");
    if (command
        && gIpcModule.appendUint(command, screen)
        && gIpcModule.appendString(command, filename)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_takeLayerScreenshot(t_ilm_const_string filename, t_ilm_layer layerid)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("ScreenShotOfLayer");
    if (command
        && gIpcModule.appendString(command, filename)
        && gIpcModule.appendUint(command, layerid)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_takeSurfaceScreenshot(t_ilm_const_string filename, t_ilm_surface surfaceid)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("ScreenShotOfSurface");
    if (command
        && gIpcModule.appendString(command, filename)
        && gIpcModule.appendUint(command, surfaceid)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_SetOptimizationMode(ilmOptimization id, ilmOptimizationMode mode)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetOptimizationMode");
    if (command
        && gIpcModule.appendUint(command, id)
        && gIpcModule.appendUint(command, mode)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_GetOptimizationMode(ilmOptimization id, ilmOptimizationMode* pMode)
{
    ilmErrorTypes returnValue = ILM_FAILED;
    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetOptimizationMode");
    if (command
        && gIpcModule.appendUint(command, id)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pMode))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerAddNotification(t_ilm_layer layer, layerNotificationFunc callback)
{
    ilmErrorTypes returnValue = ILM_FAILED;
    t_ilm_message response;
    t_ilm_message command;

    if (findLayerCallback(layer))
    {
        return ILM_ERROR_INVALID_ARGUMENTS;
    }

    response = 0;
    command = gIpcModule.createMessage("LayerAddNotification");
    if (command
        && gIpcModule.appendUint(command, layer)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        addLayerCallback(layer, callback);
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerRemoveNotification(t_ilm_layer layer)
{
    ilmErrorTypes returnValue = ILM_FAILED;
    t_ilm_message response;
    t_ilm_message command;

    if (!findLayerCallback(layer))
    {
        return ILM_ERROR_INVALID_ARGUMENTS;
    }

    response = 0;
    command = gIpcModule.createMessage("LayerRemoveNotification");
    if (command
        && gIpcModule.appendUint(command, layer)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        removeLayerCallback(layer);
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_setSynchronizedSurfaces(t_ilm_surface *pSurfaceId, const t_ilm_uint number)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSynchronizedSurfaces");
    if (pSurfaceId
        && command
        && gIpcModule.appendUintArray(command, pSurfaceId, number)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_removeSynchronizedSurfaces(t_ilm_surface *pSurfaceId, const t_ilm_uint number)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("RemoveSynchronizedSurfaces");
    if (pSurfaceId
        && command
        && gIpcModule.appendUintArray(command, pSurfaceId, number)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

