/***************************************************************************
 *
 * Copyright 2010,2011 BMW Car IT GmbH
 * Copyright (C) 2011 DENSO CORPORATION and Robert Bosch Car Multimedia Gmbh
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

#include "GraphicSystems/DrmGLESGraphicSystem.h"
#include "IlmMatrix.h"
#include "string.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"
#include "Bitmap.h"
#include "PlatformSurfaces/WaylandPlatformSurface.h"
#include "WindowSystems/WaylandBaseWindowSystem.h"

#include "wayland-server.h"

#include <errno.h>
#include <drm_fourcc.h>
#include <drm_mode.h>
#include <gbm.h>
#include <libudev.h>

static drmModeModeInfo builtin_800x480_for_Crossville = {
    31746,
    800, 859, 999, 999, 0,
    480, 507, 538, 524, 0,
    0,
    DRM_MODE_FLAG_NHSYNC | DRM_MODE_FLAG_PVSYNC,
    0,
    "800x480"
};

static void
pageFlipHandler(int /*fd*/, unsigned int /*frame*/, unsigned int /*sec*/,
                    unsigned int /*usec*/, void *data)
{
    struct DrmOutput *output = static_cast<struct DrmOutput*>(data);

    output->pageFlipPending = 0;

    if (output->current)
    {
        gbm_surface_release_buffer(output->surface, output->current->bo);
    }

    output->current = output->next;
    output->next = NULL;

    if (output->windowSystem)
    {
        output->windowSystem->finishFrame();
    }
}

static int
onDrmInput(int fd, uint32_t /*mask*/, void * /*data*/)
{
    drmEventContext evctx;
    memset(&evctx, 0, sizeof evctx);

    evctx.version = DRM_EVENT_CONTEXT_VERSION;
    evctx.page_flip_handler = pageFlipHandler;
    evctx.vblank_handler = NULL;

    drmHandleEvent(fd, &evctx);

    return 1;
}

static void
drmFbDestroyCallback(struct gbm_bo *bo, void *data)
{
    struct DrmFb *fb = (struct DrmFb*)data;
    struct gbm_device *gbm = gbm_bo_get_device(bo);

    if (fb->fbId)
        drmModeRmFB(gbm_device_get_fd(gbm), fb->fbId);

    free(data);
}

struct DrmFb*
drmFbGetFromBo(struct gbm_bo *bo, int fdDev, struct DrmOutput *output)
{
    struct DrmFb *fb = (struct DrmFb*)gbm_bo_get_user_data(bo);
    uint32_t width;
    uint32_t height;
    uint32_t stride;
    uint32_t handle;
    int ret;

    if (fb)
        return fb;

    fb = (struct DrmFb*)malloc(sizeof *fb);
    fb->bo = bo;
    fb->output = output;

    width = gbm_bo_get_width(bo);
    height = gbm_bo_get_height(bo);
    stride = gbm_bo_get_stride(bo);
    handle = gbm_bo_get_handle(bo).u32;

    ret = drmModeAddFB(fdDev, width, height, 24, 32, stride, handle, &fb->fbId);
    if (ret)
    {
        return NULL;
    }

    gbm_bo_set_user_data(bo, fb, drmFbDestroyCallback);

    return fb;
}

DrmGLESGraphicSystem::DrmGLESGraphicSystem(int windowWidth, int windowHeight,
                                     PfnShaderProgramCreator shaderProgram)
: GLESGraphicsystem(windowWidth, windowHeight, shaderProgram)
, m_gbm(NULL)
, m_fdDev(-1)
, m_crtcs(NULL)
, m_crtcsNum(0)
, m_crtcAllocator(0)
, m_connectorAllocator(0)
, m_currentOutput(NULL)
{
    LOG_DEBUG("DrmGLESGraphicSystem", "creating DrmGLESGraphicSystem");

    m_pfEglBindWaylandDisplayWL = (PFNEGLBINDWAYLANDDISPLAYWL)eglGetProcAddress("eglBindWaylandDisplayWL");
    m_pfEglUnbindWaylandDisplayWL = (PFNEGLUNBINDWAYLANDDISPLAYWL)eglGetProcAddress("eglUnbindWaylandDisplayWL");

    if (!m_pfEglBindWaylandDisplayWL ||
        !m_pfEglUnbindWaylandDisplayWL)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "Query EGL extensions failed.");
    }
}

DrmGLESGraphicSystem::~DrmGLESGraphicSystem()
{
    WaylandBaseWindowSystem* windowSystem = dynamic_cast<WaylandBaseWindowSystem*>(m_baseWindowSystem);
    struct wl_display* wlDisplay = windowSystem->getNativeDisplayHandle();

    if (NULL != m_eglContext)
    {
        eglDestroyContext(m_eglDisplay, m_eglContext);
        m_eglContext = NULL;
    }

    struct DrmOutput* output = NULL;
    wl_list_for_each(output, &m_outputList, link)
    {
        if (NULL != output->eglSurface)
        {
            eglDestroySurface(m_eglDisplay, output->eglSurface);
            output->eglSurface = NULL;
        }

        if (NULL != output->surface)
        {
            gbm_surface_destroy(output->surface);
            output->surface = NULL;
        }
        free(output);
    }

    if (wlDisplay)
        m_pfEglUnbindWaylandDisplayWL(m_eglDisplay, wlDisplay);
}

bool DrmGLESGraphicSystem::init(EGLNativeDisplayType display, EGLNativeWindowType NativeWindow)
{
    bool ret = true;
    LOG_DEBUG("DrmGLESGraphicSystem", "init..display:" << display <<
                                        ", NativeWindow:" << NativeWindow);

    m_nativeDisplay = display;
    m_nativeWindow = NativeWindow;

    wl_list_init(&m_outputList);

    m_gbm = (gbm_device*)display;
    if (m_gbm == NULL)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "gbm device is NULL.");
        return false;
    }

    m_fdDev = gbm_device_get_fd(m_gbm);
    if (m_fdDev < 0)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to get device fd.");
        return false;
    }

    m_eglDisplay = eglGetDisplay((EGLNativeDisplayType)m_gbm);
    if (m_eglDisplay == EGL_NO_DISPLAY)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to get EGL display.");
        return false;
    }

    EGLint major;
    EGLint minor;
    if (!eglInitialize(m_eglDisplay, &major, &minor))
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to initialising EGL.");
        return false;
    }

    static const EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 1,
        EGL_GREEN_SIZE, 1,
        EGL_BLUE_SIZE, 1,
        EGL_ALPHA_SIZE, 0,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    EGLint n = 0;
    if (!eglChooseConfig(m_eglDisplay, configAttribs, &m_eglConfig, 1, &n) || n != 1)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to choose config.");
        return false;
    }

    if (!initializeSystem())
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to initialize system.");
        return false;
    }

    if (!GLESGraphicsystem::initOpenGLES(m_windowWidth, m_windowHeight))
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to initialize gles.");
        return false;
    }

    return ret;
}

void DrmGLESGraphicSystem::activateGraphicContext()
{
}

void DrmGLESGraphicSystem::updateScreenList(LmScreenList& screenList)
{
    LmScreenListIterator iter = screenList.begin();
    LmScreenListIterator iterEnd = screenList.end();
    for (; iter != iterEnd; ++iter)
    {
        delete (*iter);
    }
    screenList.clear();

    struct DrmOutput* output = NULL;
    wl_list_for_each(output, &m_outputList, link)
    {
        LmScreen* lmScreen = new LmScreen(output->screenID, "");
        screenList.push_back(lmScreen);
    }
}

void DrmGLESGraphicSystem::switchScreen(uint screenID)
{
    // Actually, when renderSWLayers is called, rendering target buffer is switched
    // because of avoiding overhead of switching display.
    struct DrmOutput* output = NULL;
    wl_list_for_each(output, &m_outputList, link)
    {
        if (output->screenID != screenID)
        {
            continue;
        }

        LOG_DEBUG("DrmGLESGraphicSystem", "switch screen:" << m_currentOutput->screenID);
        m_currentOutput = output;
        drmOutputPrepareRender(m_currentOutput);
        break;
    }
}

bool DrmGLESGraphicSystem::initializeSystem()
{
    LOG_DEBUG("DrmGLESGraphicSystem", "initializeSystem IN");

    WaylandBaseWindowSystem* windowSystem = dynamic_cast<WaylandBaseWindowSystem*>(m_baseWindowSystem);
    struct wl_display* wlDisplay = windowSystem->getNativeDisplayHandle();

    // bind display
    m_pfEglBindWaylandDisplayWL(m_eglDisplay, wlDisplay);

    if (!createOutputs())
        return false;

    if (!eglBindAPI(EGL_OPENGL_ES_API))
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to bind api EGL_OPENGL_ES_API.");
        return false;
    }

    EGLint contextAttrs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, contextAttrs);
    if (!m_eglContext)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to create EGL context.");
        return false;
    }

    struct DrmOutput *output = wl_container_of(m_outputList.prev, output, link);

    if (!eglMakeCurrent(m_eglDisplay, output->eglSurface, output->eglSurface, m_eglContext))
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to make context current.");
        return false;
    }

    struct wl_event_loop *loop = wl_display_get_event_loop(wlDisplay);
    wl_event_loop_add_fd(loop, m_fdDev, WL_EVENT_READABLE, onDrmInput, NULL);

    LOG_DEBUG("DrmGLESGraphicSystem", "initializeSystem OUT");

    return true;
}

bool DrmGLESGraphicSystem::createOutputs()
{
    LOG_DEBUG("DrmGLESGraphicSystem", "createOutputs IN");

    drmModeConnector* connector;
    drmModeRes*       resources;
    int x = 0;
    int y = 0;

    resources = drmModeGetResources(m_fdDev);
    if (!resources)
    {
        LOG_DEBUG("DrmGLESGraphicSystem", "drmModeGetResources failed.");
        return false;
    }

    m_crtcs = (uint32_t*)calloc(resources->count_crtcs, sizeof(uint32_t));
    if (!m_crtcs)
    {
        drmModeFreeResources(resources);
        return false;
    }
    m_crtcsNum = resources->count_crtcs;

    memcpy(m_crtcs, resources->crtcs, sizeof(uint32_t) * m_crtcsNum);

    for (int ii = 0; ii < resources->count_connectors; ++ii)
    {
        connector = drmModeGetConnector(m_fdDev, resources->connectors[ii]);
        if (connector == NULL)
            continue;

        if (connector->connection == DRM_MODE_CONNECTED)
        {
            if (createOutputForConnector(resources, connector, x, y) < 0)
            {
                drmModeFreeConnector(connector);
                continue;
            }

            //x += container_of(m_outputList.prev, struct DrmOutput, link)->currentMode->width;
        }

        drmModeFreeConnector(connector);
    }

    if (wl_list_empty(&m_outputList))
    {
        LOG_ERROR("DrmGLESGraphicSystem", "DrmOutput list is empty.");
        drmModeFreeResources(resources);
        return false;
    }

    drmModeFreeResources(resources);

    LOG_DEBUG("DrmGLESGraphicSystem", "createOutputs OUT");
    return true;
}

int DrmGLESGraphicSystem::createOutputForConnector(drmModeRes* resources,
                                                    drmModeConnector* connector,
                                                    int, int)
{
    LOG_DEBUG("DrmGLESGraphicSystem", "createOutputForConnector IN");
    int ii;
    drmModeEncoder* encoder;
    struct DrmMode* drmMode = NULL;

    encoder = drmModeGetEncoder(m_fdDev, connector->encoders[0]);
    if (encoder == NULL)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "No encoder for connector.");
        return -1;
    }

    for (ii = 0; ii < resources->count_crtcs; ++ii)
    {
        if (encoder->possible_crtcs & (1 << ii) &&
            !(m_crtcAllocator & (1 << resources->crtcs[ii])))
        {
            break;
        }
    }
    if (ii == resources->count_crtcs)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "No usable crtc for encoder.");
        return -1;
    }

    DrmOutput* output = (DrmOutput*)malloc(sizeof *output);
    if (output == NULL)
    {
        drmModeFreeEncoder(encoder);
        return -1;
    }
    memset(output, 0x00, sizeof *output);

    output->windowSystem = m_baseWindowSystem;
    output->fdDev = m_fdDev;
    output->crtcID = resources->crtcs[ii];
    output->connectorID = connector->connector_id;

    m_crtcAllocator      |= (1 << output->crtcID);
    m_connectorAllocator |= (1 << output->connectorID);

    wl_list_init(&output->modeList);

    output->orgCrtc = drmModeGetCrtc(m_fdDev, output->crtcID);
    drmModeFreeEncoder(encoder);

    for (ii = 0; ii < connector->count_modes; ++ii)
    {
        if (drmOutputAddMode(output, &connector->modes[ii]))
            goto err_free;
    }

    if (connector->count_modes == 0)
    {
        if (drmOutputAddMode(output, &builtin_800x480_for_Crossville))
            goto err_free;
    }

    drmMode = wl_container_of(output->modeList.next, drmMode, link);
    output->currentMode = drmMode;
    drmMode->flags = WL_OUTPUT_MODE_CURRENT | WL_OUTPUT_MODE_PREFERRED;

    output->surface = gbm_surface_create(m_gbm,
                                output->currentMode->width,
                                output->currentMode->height,
                                GBM_BO_FORMAT_XRGB8888,
                                GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);
    if (!output->surface)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to create gbm surface.");
        goto err_gbm_surface;
    }

    output->eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig,
        (EGLNativeWindowType)output->surface, NULL);
    if (output->eglSurface == EGL_NO_SURFACE)
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to create egl surface.");
        goto err_egl_surface;
    }

    output->screenID = (uint)wl_list_length(&m_outputList);
    m_currentOutput = output;

    wl_list_insert(m_outputList.prev, &output->link);

    LOG_DEBUG("DrmGLESGraphicSystem", "createOutputForConnector OUT (NORMAL)");
    return 0;

err_egl_surface:
    if (output->eglSurface)
        eglDestroySurface(m_eglDisplay, output->eglSurface);

err_gbm_surface:
    if (output->surface)
        gbm_surface_destroy(output->surface);

err_free:
    drmModeFreeCrtc(output->orgCrtc);
    m_crtcAllocator &= ~(1 << output->crtcID);
    m_connectorAllocator &= ~(1 << output->connectorID);

    free(output);
    LOG_DEBUG("DrmGLESGraphicSystem", "createOutputForConnector OUT (ERROR)");
    return -1;
}

int DrmGLESGraphicSystem::drmOutputAddMode(struct DrmOutput* output, drmModeModeInfo* info)
{
    struct DrmMode* mode;

    mode = (struct DrmMode*)malloc(sizeof *mode);
    if (mode == NULL)
        return -1;

    mode->flags = 0;
    mode->width = info->hdisplay;
    mode->height = info->vdisplay;
    mode->refresh = info->vrefresh;
    mode->modeInfo = *info;
    wl_list_insert(output->modeList.prev, &mode->link);

    return 0;
}

int DrmGLESGraphicSystem::drmOutputPrepareRender(struct DrmOutput* output)
{
    if (!eglMakeCurrent(m_eglDisplay, output->eglSurface, output->eglSurface, m_eglContext))
    {
        LOG_ERROR("DrmGLESGraphicSystem", "failed to make current.");
        return -1;
    }
    return 0;
}

void DrmGLESGraphicSystem::swapBuffers()
{
    LOG_DEBUG("DrmGLESGraphicSystem", "swapBuffers IN");

    struct DrmOutput* output = NULL;

    wl_list_for_each(output, &m_outputList, link)
    {
        if (output != m_currentOutput)
        {
            continue;
        }

        if (!output->next)
        {
            eglSwapBuffers(m_eglDisplay, output->eglSurface);
            struct gbm_bo *bo = gbm_surface_lock_front_buffer(output->surface);
            if (!bo)
                break;

            output->next = drmFbGetFromBo(bo, m_fdDev, output);
            if (!output->next)
            {
                gbm_surface_release_buffer(output->surface, bo);
                break;
            }
        }

        if (!output->current)
        {
            if (drmModeSetCrtc(m_fdDev, output->crtcID,
                                    output->next->fbId, 0, 0,
                                    &output->connectorID, 1,
                                    &output->currentMode->modeInfo))
            {
                LOG_ERROR("DrmGLESGraphicSystem", "failed to set mode in swapBuffers.");
                break;
            }
        }

        if (drmModePageFlip(m_fdDev, output->crtcID, output->next->fbId,
            DRM_MODE_PAGE_FLIP_EVENT, output) < 0)
        {
            LOG_ERROR("DrmGLESGraphicSystem", "queueing pageflip failed");
            break;
        }

        output->pageFlipPending = 1;
        break;
    }

    LOG_DEBUG("DrmGLESGraphicSystem", "swapBuffers OUT");
}
