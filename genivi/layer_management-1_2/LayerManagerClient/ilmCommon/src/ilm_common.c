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
#include "ilm_tools.h"
#include "ilm_types.h"
#include "ilm_configuration.h"

#include "IpcModuleLoader.h"
#include "ObjectType.h"
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <semaphore.h>
#include <unistd.h>

/*
 *=============================================================================
 * global vars
 *=============================================================================
 */
/* automatically gets assigned argv[0] */
extern char *__progname;

/* available to all client APIs, exported in ilm_common.h */
struct IpcModule gIpcModule;

/* internal use */
static t_ilm_bool gInitialized = ILM_FALSE;
static pthread_t gReceiveThread;
static pthread_t gNotificationThread;

/*
 *=============================================================================
 * implementation
 *=============================================================================
 */
ilmErrorTypes ilm_init()
{
    ilmErrorTypes result = ILM_FAILED;
    t_ilm_message response = 0;
    t_ilm_message command;

    if (gInitialized)
    {
        printf("ilm_init() was called, but ilmClientLib is already initialized. returning success, but initialization was skipped this time.\n");
        return ILM_SUCCESS;
    }

    initNotificationCallbacks();

    if (loadIpcModule(&gIpcModule))
    {
        int pid = getpid();

        if (gIpcModule.initClientMode())
        {
            pthread_attr_t notificationThreadAttributes;
            int ret;

            result = ILM_SUCCESS;

            init_msg_queue(&notificationQueue, MAX_THREAD_SYNC_QUEUE_SIZE);

            init_msg_queue(&incomingQueue, MAX_THREAD_SYNC_QUEUE_SIZE);

            if (notificationQueue.maxSize == 0)
            {
                printf("failed to allocate queue\n");
                return result;
            }

            pthread_mutex_init(&gSendReceiveLock, NULL);

            pthread_attr_init(&notificationThreadAttributes);
            pthread_attr_setdetachstate(&notificationThreadAttributes,
                                        PTHREAD_CREATE_JOINABLE);

            ret = pthread_create(&gReceiveThread,
                                    &notificationThreadAttributes,
                                    receiveThreadLoop,
                                    NULL);
            if (0 != ret)
            {
                result = ILM_FAILED;
                printf("Failed to start internal receive thread. returned %d = %s\n", ret, (ret == EAGAIN ? "EAGAIN" : "EINVAL"));
                return result;
            }

            ret = pthread_create(&gNotificationThread,
                                    &notificationThreadAttributes,
                                    notificationThreadLoop,
                                    NULL);
            if (0 != ret)
            {
                result = ILM_FAILED;
                printf("Failed to start internal notification thread. returned %d = %s\n", ret, (ret == EAGAIN ? "EAGAIN" : "EINVAL"));
                return result;
            }
        }
        else
        {
            result = ILM_FAILED;
            printf("Failed to initialize Client Ipc Module");
            return result;
        }

        command = gIpcModule.createMessage("ServiceConnect");
        if (command
                && gIpcModule.appendUint(command, pid)
                && gIpcModule.appendString(command, __progname)
                && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &result))
        {
            result = ILM_SUCCESS;
        }
        else
        {
            printf("Failed to connect to LayerManagerService.");
        }
        gIpcModule.destroyMessage(response);
        gIpcModule.destroyMessage(command);
    }

    gInitialized = (result == ILM_SUCCESS) ? ILM_TRUE : ILM_FALSE;

    return result;
}

t_ilm_bool ilm_isInitialized()
{
    return gInitialized;
}

ilmErrorTypes ilm_commitChanges()
{
    ilmErrorTypes returnValue = ILM_FAILED;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("CommitChanges");

    if (command
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &returnValue))
    {
        returnValue = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);
    return returnValue;
}

ilmErrorTypes ilm_destroy()
{
    ilmErrorTypes result = ILM_FAILED;
    void* threadReturnValue = NULL;

    t_ilm_message response = 0;
    t_ilm_message command = gIpcModule.createMessage("ServiceDisconnect");
    if (command
        && gIpcModule.appendUint(command, getpid())
        && sendAndWaitForResponse(command, &response, RESPONSE_TIMEOUT_IN_MS, &result))
    {
        result = ILM_SUCCESS;
    }
    gIpcModule.destroyMessage(response);
    gIpcModule.destroyMessage(command);

    /* cancel worker threads */
    pthread_cancel(gReceiveThread);
    pthread_cancel(gNotificationThread);

    pthread_join(gReceiveThread, &threadReturnValue);
    pthread_join(gNotificationThread, &threadReturnValue);

    pthread_mutex_unlock(&gSendReceiveLock);

    pthread_mutex_destroy(&gSendReceiveLock);

    gIpcModule.destroy();

    destroy_msg_queue(&notificationQueue);
    destroy_msg_queue(&incomingQueue);

    gInitialized = ILM_FALSE;

    return result;
}
