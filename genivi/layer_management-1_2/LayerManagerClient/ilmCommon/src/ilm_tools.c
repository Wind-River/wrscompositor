/***************************************************************************
 *
 * Copyright 2013 BMW Car IT GmbH
 *
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

#include "ilm_tools.h"
#include "ilm_types.h"

#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <IpcModuleLoader.h>
#include <stdio.h>
#include <errno.h>
#include <sys/time.h>

/* available to all client APIs, exported in ilm_tools.h */
t_ilm_msg_queue notificationQueue;
t_ilm_msg_queue incomingQueue;
pthread_mutex_t gSendReceiveLock;

/*
 *=============================================================================
 * Implementation of thread-safe circular queue for local use
 *=============================================================================
 */
void init_msg_queue(t_ilm_msg_queue* pQueue, t_ilm_uint maxSize)
{
    pQueue->maxSize = maxSize;
    pQueue->messages = malloc(sizeof(t_ilm_message) * maxSize);

    pQueue->size = 0;
    pQueue->readPos = 0;
    pQueue->writePos = 0;

    pthread_mutex_init(&pQueue->queueMutex, NULL);
    sem_init(&pQueue->readBlockSemaphore, 0, 0);
}

t_ilm_bool msg_enqueue(t_ilm_msg_queue* pQueue, t_ilm_message message)
{
    pthread_mutex_lock(&pQueue->queueMutex);

    if (pQueue->size < pQueue->maxSize)
    {
        ++pQueue->size;
        pQueue->messages[pQueue->writePos] = message;
        pQueue->writePos = (pQueue->writePos + 1) % pQueue->maxSize;

        /* wakeup a blocked dequeue reqquest */
        sem_post(&pQueue->readBlockSemaphore);

        pthread_mutex_unlock(&pQueue->queueMutex);
        return ILM_TRUE;
    }

    pthread_mutex_unlock(&pQueue->queueMutex);
    return ILM_FALSE;
}

t_ilm_message msg_dequeue(t_ilm_msg_queue* pQueue)
{
    t_ilm_message result = NULL;
    /* wait until a message is available */
    sem_wait(&pQueue->readBlockSemaphore);

    pthread_mutex_lock(&pQueue->queueMutex);

    if (pQueue->size > 0)
    {
        --pQueue->size;
        result = pQueue->messages[pQueue->readPos];
        pQueue->readPos = (pQueue->readPos + 1) % pQueue->maxSize;
    }

    pthread_mutex_unlock(&pQueue->queueMutex);
    return result;
}


void destroy_msg_queue(t_ilm_msg_queue* pQueue)
{
    if (pQueue->maxSize > 0)
    {
        pQueue->maxSize = 0;
        pQueue->size = 0;
        free(pQueue->messages);
    }
}



/*
 *=============================================================================
 * notification management
 *=============================================================================
 */
static struct LayerCallback gLayerNotificationCallbacks[MAX_CALLBACK_COUNT];
static struct SurfaceCallback gSurfaceNotificationCallbacks[MAX_CALLBACK_COUNT];

void initNotificationCallbacks()
{
    int i = 0;
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        gLayerNotificationCallbacks[i].id = INVALID_ID;
        gLayerNotificationCallbacks[i].callback = NULL;
        gSurfaceNotificationCallbacks[i].id = INVALID_ID;
        gSurfaceNotificationCallbacks[i].callback = NULL;
    }
}

layerNotificationFunc getLayerNotificationCallback(t_ilm_layer layer)
{
    int i = 0;
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        if (gLayerNotificationCallbacks[i].id == layer)
        {
            return gLayerNotificationCallbacks[i].callback;
        }
    }
    return NULL;
}

surfaceNotificationFunc getSurfaceNotificationCallback(t_ilm_surface surface)
{
    int i = 0;
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        if (gSurfaceNotificationCallbacks[i].id == surface)
        {
            return gSurfaceNotificationCallbacks[i].callback;
        }
    }
    return NULL;
}

t_ilm_bool findLayerCallback(t_ilm_layer layer)
{
    int i = 0;

    /* try to overwrite existing entry for layer id */
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        if (gLayerNotificationCallbacks[i].id == layer)
        {
            return ILM_TRUE;
        }
    }
    return ILM_FALSE;
}

t_ilm_bool addLayerCallback(t_ilm_layer layer, layerNotificationFunc func)
{
    int i = 0;

    if (findLayerCallback(layer))
    {
        return ILM_FALSE;
    }

    /* find free slot and store callback */
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        if (gLayerNotificationCallbacks[i].id == INVALID_ID)
        {
            gLayerNotificationCallbacks[i].id = layer;
            gLayerNotificationCallbacks[i].callback = func;
            return ILM_TRUE;
        }
    }
    printf("DbusIpcModule: addLayerCallback() failed. no free slots.");
    return ILM_FALSE;
}

t_ilm_bool findSurfaceCallback(t_ilm_surface surface)
{
    int i = 0;

    /* try to overwrite existing entry for layer id */
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        if (gSurfaceNotificationCallbacks[i].id == surface)
        {
            return ILM_TRUE;
        }
    }
    return ILM_FALSE;
}

t_ilm_bool addSurfaceCallback(t_ilm_surface surface, surfaceNotificationFunc func)
{
    int i = 0;

    if (findSurfaceCallback(surface))
    {
        return ILM_FALSE;
    }

    /* find free slot and store callback */
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        if (gSurfaceNotificationCallbacks[i].id == INVALID_ID)
        {
            gSurfaceNotificationCallbacks[i].id = surface;
            gSurfaceNotificationCallbacks[i].callback = func;
            return ILM_TRUE;
        }
    }
    printf("DbusIpcModule: addSurfaceCallback() failed. no free slots.");
    return ILM_FALSE;
}

void removeLayerCallback(t_ilm_layer layer)
{
    int i = 0;
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        if (gLayerNotificationCallbacks[i].id == layer)
        {
            gLayerNotificationCallbacks[i].id = INVALID_ID;
            gLayerNotificationCallbacks[i].callback = NULL;
            return;
        }
    }
}

void removeSurfaceCallback(t_ilm_surface layer)
{
    int i = 0;
    for (i = 0; i < MAX_CALLBACK_COUNT; ++i)
    {
        if (gSurfaceNotificationCallbacks[i].id == layer)
        {
            gSurfaceNotificationCallbacks[i].id = INVALID_ID;
            gSurfaceNotificationCallbacks[i].callback = NULL;
            return;
        }
    }
}


/*
 *=============================================================================
 * handling of internal notification thread for dispatching notifications
 * Note: notification callbacks may be blocked by client, but receive thread
 * must not be blocked
 *=============================================================================
 */

void* notificationThreadLoop(void* param)
{
    t_ilm_message notification;

    (void)param;

    while (NULL != (notification = msg_dequeue(&notificationQueue)))
    {
        t_ilm_const_string name = gIpcModule.getMessageName(notification);

        /* this depends on message name, but it is fast */
        if ('L' == name[15])
        {
            t_ilm_uint id;
            t_ilm_uint mask;
            struct ilmLayerProperties properties;
            layerNotificationFunc func;

            gIpcModule.getUint(notification, &id);
            gIpcModule.getUint(notification, &mask);
            gIpcModule.getDouble(notification, &properties.opacity);
            gIpcModule.getUint(notification, &properties.sourceX);
            gIpcModule.getUint(notification, &properties.sourceY);
            gIpcModule.getUint(notification, &properties.sourceWidth);
            gIpcModule.getUint(notification, &properties.sourceHeight);
            gIpcModule.getUint(notification, &properties.origSourceWidth);
            gIpcModule.getUint(notification, &properties.origSourceHeight);
            gIpcModule.getUint(notification, &properties.destX);
            gIpcModule.getUint(notification, &properties.destY);
            gIpcModule.getUint(notification, &properties.destWidth);
            gIpcModule.getUint(notification, &properties.destHeight);
            gIpcModule.getUint(notification, &properties.orientation);
            gIpcModule.getBool(notification, &properties.visibility);
            gIpcModule.getUint(notification, &properties.type);
            gIpcModule.getBool(notification, &properties.chromaKeyEnabled);
            gIpcModule.getUint(notification, &properties.chromaKeyRed);
            gIpcModule.getUint(notification, &properties.chromaKeyGreen);
            gIpcModule.getUint(notification, &properties.chromaKeyBlue);
            gIpcModule.getInt(notification, &properties.creatorPid);

            func = getLayerNotificationCallback(id);
            if (func)
            {
                (*func)(id, &properties, mask);
            }
            else
            {
                fprintf(stderr, "notification for layer %d received, but no callback set\n", id);
            }
        }

        if ('S' == name[15])
        {
            t_ilm_uint id;
            t_ilm_uint mask;
            struct ilmSurfaceProperties properties;
            surfaceNotificationFunc func;

            gIpcModule.getUint(notification, &id);
            gIpcModule.getUint(notification, &mask);
            gIpcModule.getDouble(notification, &properties.opacity);
            gIpcModule.getUint(notification, &properties.sourceX);
            gIpcModule.getUint(notification, &properties.sourceY);
            gIpcModule.getUint(notification, &properties.sourceWidth);
            gIpcModule.getUint(notification, &properties.sourceHeight);
            gIpcModule.getUint(notification, &properties.origSourceWidth);
            gIpcModule.getUint(notification, &properties.origSourceHeight);
            gIpcModule.getUint(notification, &properties.destX);
            gIpcModule.getUint(notification, &properties.destY);
            gIpcModule.getUint(notification, &properties.destWidth);
            gIpcModule.getUint(notification, &properties.destHeight);
            gIpcModule.getUint(notification, &properties.orientation);
            gIpcModule.getBool(notification, &properties.visibility);
            gIpcModule.getUint(notification, &properties.frameCounter);
            gIpcModule.getUint(notification, &properties.drawCounter);
            gIpcModule.getUint(notification, &properties.updateCounter);
            gIpcModule.getUint(notification, &properties.pixelformat);
            gIpcModule.getUint(notification, &properties.nativeSurface);
            gIpcModule.getUint(notification, &properties.inputDevicesAcceptance);
            gIpcModule.getBool(notification, &properties.chromaKeyEnabled);
            gIpcModule.getUint(notification, &properties.chromaKeyRed);
            gIpcModule.getUint(notification, &properties.chromaKeyGreen);
            gIpcModule.getUint(notification, &properties.chromaKeyBlue);
            gIpcModule.getInt(notification, &properties.creatorPid);

            func = getSurfaceNotificationCallback(id);
            if (func)
            {
                (*func)(id, &properties, mask);
            }
            else
            {
                fprintf(stderr, "notification for surface %d received, but no callback set\n", id);
            }
        }
        gIpcModule.destroyMessage(notification);
    }
    return NULL;
}


/*
 *=============================================================================
 * handling of internal receive thread for event handling
 *=============================================================================
 */

void* receiveThreadLoop(void* param)
{
    t_ilm_bool running = ILM_TRUE;

    (void)param;

    while (running)
    {
        t_ilm_message message = gIpcModule.receive(RECEIVE_TIMEOUT_IN_MS);
        t_ilm_message_type messageType = gIpcModule.getMessageType(message);
        switch (messageType)
        {
        case IpcMessageTypeNotification:
            if (ILM_FALSE == msg_enqueue(&notificationQueue, message))
            {
                if (EAGAIN == errno)
                {
                    printf("Notification queue full, dropped notification %s\n", gIpcModule.getMessageName(message));
                }
            }
            break;

        case IpcMessageTypeCommand:
        case IpcMessageTypeError:
            if (ILM_FALSE == msg_enqueue(&incomingQueue, message))
            {
                if (EAGAIN == errno)
                {
                    printf("Incoming queue full, dropped message %s\n", gIpcModule.getMessageName(message));
                }
            }
            break;

        case IpcMessageTypeNone:
            break;

        default:
            printf("ilmClient: discarded unexpected message (type: %d)\n", (int)messageType);
            gIpcModule.destroyMessage(message);
            break;
        }
    }

    return NULL;
}

void calculateTimeout(struct timeval* currentTime, int giventimeout, struct timespec* timeout)
{
    /* nanoseconds is old value in nanoseconds + the given milliseconds as nanoseconds */
    t_ilm_ulong newNanoSeconds = currentTime->tv_usec * 1000 + giventimeout * (1000 * 1000);

    /* only use non full seconds, otherwise overflow! */
    timeout->tv_nsec = newNanoSeconds % (1000000000);

    /* new seconds are old seconds + full seconds from new nanoseconds part */
    timeout->tv_sec = currentTime->tv_sec + (newNanoSeconds / 1000000000);
}

t_ilm_bool sendAndWaitForResponse(t_ilm_message command, t_ilm_message* response, int timeoutInMs, ilmErrorTypes* error)
{
    t_ilm_message_type responseType = IpcMessageTypeNone;

    struct timeval tv;
    struct timespec ts;

    (void)timeoutInMs; /* suppress warning */

    gettimeofday(&tv, NULL);
    calculateTimeout(&tv, 1000000, &ts);

    *response = 0;

    /* send / receive may only be performed by one thread at a time */
    pthread_mutex_lock(&gSendReceiveLock);

    if (gIpcModule.sendToService(command))
    {
        if (NULL == (*response = msg_dequeue(&incomingQueue)))
        {
            *error = ILM_ERROR_ON_CONNECTION;
        }
        else
        {
            responseType = gIpcModule.getMessageType(*response);
            switch (responseType)
            {
            case IpcMessageTypeCommand:
                break;

            case IpcMessageTypeError:
                gIpcModule.getUint(*response, error);
                free(*response);
                *response = 0;
                break;

            default:
                fprintf(stderr, "waitForResponse: LayerManagerService returned unexpected message type %d\n", responseType);
                free(*response);
                *response = 0;
                *error = ILM_ERROR_UNEXPECTED_MESSAGE;
                break;
            }
        }
    }
    pthread_mutex_unlock(&gSendReceiveLock);

    return (0 != *response);
}
