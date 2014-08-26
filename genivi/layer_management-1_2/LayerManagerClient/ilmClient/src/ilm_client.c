/**************************************************************************
 *
 * Copyright 2012,2013 BMW Car IT GmbH
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
#include "ilm_client.h"

ilmErrorTypes ilm_getPropertiesOfSurface(t_ilm_uint surfaceID, struct ilmSurfaceProperties* pSurfaceProperties)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetPropertiesOfSurface");

    if (pSurfaceProperties
        && command
        && gIpcModule.appendUint(command, surfaceID)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getDouble(response, &pSurfaceProperties->opacity)
        && gIpcModule.getUint(response, &pSurfaceProperties->sourceX)
        && gIpcModule.getUint(response, &pSurfaceProperties->sourceY)
        && gIpcModule.getUint(response, &pSurfaceProperties->sourceWidth)
        && gIpcModule.getUint(response, &pSurfaceProperties->sourceHeight)
        && gIpcModule.getUint(response, &pSurfaceProperties->origSourceWidth)
        && gIpcModule.getUint(response, &pSurfaceProperties->origSourceHeight)
        && gIpcModule.getUint(response, &pSurfaceProperties->destX)
        && gIpcModule.getUint(response, &pSurfaceProperties->destY)
        && gIpcModule.getUint(response, &pSurfaceProperties->destWidth)
        && gIpcModule.getUint(response, &pSurfaceProperties->destHeight)
        && gIpcModule.getUint(response, &pSurfaceProperties->orientation)
        && gIpcModule.getBool(response, &pSurfaceProperties->visibility)
        && gIpcModule.getUint(response, &pSurfaceProperties->frameCounter)
        && gIpcModule.getUint(response, &pSurfaceProperties->drawCounter)
        && gIpcModule.getUint(response, &pSurfaceProperties->updateCounter)
        && gIpcModule.getUint(response, &pSurfaceProperties->pixelformat)
        && gIpcModule.getUint(response, &pSurfaceProperties->nativeSurface)
        && gIpcModule.getUint(response, &pSurfaceProperties->inputDevicesAcceptance)
        && gIpcModule.getBool(response, &pSurfaceProperties->chromaKeyEnabled)
        && gIpcModule.getUint(response, &pSurfaceProperties->chromaKeyRed)
        && gIpcModule.getUint(response, &pSurfaceProperties->chromaKeyGreen)
        && gIpcModule.getUint(response, &pSurfaceProperties->chromaKeyBlue)
        && gIpcModule.getInt(response, &pSurfaceProperties->creatorPid))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}


ilmErrorTypes ilm_getScreenResolution(t_ilm_uint screenID, t_ilm_uint* pWidth, t_ilm_uint* pHeight)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetScreenResolution");
    if (pWidth && pHeight
        && command
        && gIpcModule.appendUint(command, screenID)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getUint(response, pWidth)
        && gIpcModule.getUint(response, pHeight))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerAddSurface(t_ilm_layer layerId, t_ilm_surface surfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("AddSurfaceToLayer");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_layerRemoveSurface(t_ilm_layer layerId, t_ilm_surface surfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("RemoveSurfaceFromLayer");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendUint(command, layerId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceAddNotification(t_ilm_surface surface, surfaceNotificationFunc callback)
{
    ilmErrorTypes returnValue = ILM_FAILED;
    t_ilm_message response;
    t_ilm_message command;

    if (findSurfaceCallback(surface))
    {
        return ILM_ERROR_INVALID_ARGUMENTS;
    }

    response = 0;
    command = gIpcModule.createMessage("SurfaceAddNotification");
    if (command
        && gIpcModule.appendUint(command, surface)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        addSurfaceCallback(surface, callback);
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceCreate(t_ilm_nativehandle nativehandle, t_ilm_int width, t_ilm_int height,
                                ilmPixelFormat pixelFormat, t_ilm_surface* pSurfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    if (pSurfaceId && (INVALID_ID != *pSurfaceId))
    {
        t_ilm_message response = 0;
        t_ilm_message command = gIpcModule.createMessage("CreateSurfaceFromId");
        if (command
            && gIpcModule.appendUint(command, nativehandle)
            && gIpcModule.appendUint(command, width)
            && gIpcModule.appendUint(command, height)
            && gIpcModule.appendUint(command, pixelFormat)
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
        t_ilm_message command = gIpcModule.createMessage("CreateSurface");
        if (command
            && gIpcModule.appendUint(command, nativehandle)
            && gIpcModule.appendUint(command, width)
            && gIpcModule.appendUint(command, height)
            && gIpcModule.appendUint(command, pixelFormat)
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

ilmErrorTypes ilm_surfaceGetDimension(t_ilm_surface surfaceId, t_ilm_uint *pDimension)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetSurfaceDimension");
    if (pDimension
        && command
        && gIpcModule.appendUint(command, surfaceId)
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

ilmErrorTypes ilm_surfaceGetVisibility(t_ilm_surface surfaceId, t_ilm_bool *pVisibility)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("GetSurfaceVisibility");
    if (pVisibility
        && command
        && gIpcModule.appendUint(command, surfaceId)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue)
        && gIpcModule.getBool(response, pVisibility))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceRemove(t_ilm_surface surfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("RemoveSurface");
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

ilmErrorTypes ilm_surfaceRemoveNativeContent(t_ilm_surface surfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("RemoveSurfaceNativeContent");
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

ilmErrorTypes ilm_surfaceRemoveNotification(t_ilm_surface surface)
{
    ilmErrorTypes returnValue = ILM_FAILED;
    t_ilm_message response;
    t_ilm_message command;

    if (!findSurfaceCallback(surface))
    {
        return ILM_ERROR_INVALID_ARGUMENTS;
    }

    response = 0;
    command = gIpcModule.createMessage("SurfaceRemoveNotification");
    if (command
        && gIpcModule.appendUint(command, surface)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        removeSurfaceCallback(surface);
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetNativeContent(t_ilm_nativehandle nativehandle, t_ilm_int width, t_ilm_int height,
                                            ilmPixelFormat pixelFormat, t_ilm_surface surfaceId)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceNativeContent");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendUint(command, nativehandle)
        && gIpcModule.appendUint(command, width)
        && gIpcModule.appendUint(command, height)
        && gIpcModule.appendUint(command, pixelFormat)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_surfaceSetSourceRectangle(t_ilm_surface surfaceId, t_ilm_int x, t_ilm_int y, t_ilm_int width, t_ilm_int height)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("SetSurfaceSourceRegion");
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

ilmErrorTypes ilm_UpdateInputEventAcceptanceOn(t_ilm_surface surfaceId, ilmInputDevice devices, t_ilm_bool acceptance)
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("UpdateInputEventAcceptanceOn");
    if (command
        && gIpcModule.appendUint(command, surfaceId)
        && gIpcModule.appendUint(command, devices)
        && gIpcModule.appendBool(command, acceptance)
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}
