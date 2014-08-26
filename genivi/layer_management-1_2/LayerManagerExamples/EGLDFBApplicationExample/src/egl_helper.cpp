/**************************************************************************
 *
 * Copyright 2010,2011 BMW Car IT GmbH
 * Copyright (c) 2013 DirectFB integrated media GmbH
 * Copyright (c) 2013 Renesas Solutions Corp.
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

#define EGL_EGLEXT_PROTOTYPES

#include <directfb.h>

#include <EGL/egldfbext.h>

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>


#define GL_GLEXT_PROTOTYPES

#include <GLES2/gl2.h>


#include "egl_helper.h"
#include "LayerScene.h"
#include "ilm_control.h"


typedef struct t_eglContextStruct {
     EGLDisplay display;
     EGLConfig  configs[2];
     EGLSurface surface;
     EGLContext context;
} EGLContextStruct;

typedef struct t_dfbContextStruct {
     IDirectFB             *dfb;
     IDirectFBDisplayLayer *layer;
     IDirectFBWindow       *window;
     IDirectFBSurface      *primary;
     DFBDimension           size;
} DFBContextStruct;

static EGLContextStruct g_eglContextStruct;
static DFBContextStruct g_dfbContextStruct;

EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

static DFBResult InitDFB( DFBContextStruct *test, int width, int height )
{
     DFBResult            ret;
     DFBWindowDescription wdesc;

     ret = DirectFBInit( 0, NULL );
     if (ret) {
          printf( "EGLDFBApplicationExample: DirectFBInit() failed! (ret=%d)\n", ret );
          return ret;
     }

     ret = DirectFBCreate( &test->dfb );
     if (ret) {
          printf( "EGLDFBApplicationExample: DirectFBCreate() failed! (ret=%d)\n", ret );
          return ret;
     }

     ret = test->dfb->GetDisplayLayer( test->dfb, DLID_PRIMARY, &test->layer );
     if (ret) {
          printf( "EGLDFBApplicationExample: GetDisplayLayer() failed! (ret=%d)\n", ret );
          return ret;
     }

     test->layer->SetCooperativeLevel( test->layer, DLSCL_ADMINISTRATIVE );
     test->layer->EnableCursor ( test->layer, 1 );

     wdesc.flags  = (DFBWindowDescriptionFlags)( DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS );
     wdesc.caps   = DWCAPS_ALPHACHANNEL;
     wdesc.posx   = 0;
     wdesc.posy   = 0;
     wdesc.width  = width;
     wdesc.height = height;

     ret = test->layer->CreateWindow( test->layer, &wdesc, &test->window );
     if (ret) {
          printf( "EGLDFBApplicationExample: IDirectFB::CreateWindow() failed! (ret=%d)\n", ret );
          return ret;
     }

     ret = test->window->GetSurface( test->window, &test->primary );
     if (ret) {
          printf( "EGLDFBApplicationExample: IDirectFB::CreateSurface( DSCAPS_PRIMARY | DSCAPS_FLIPPING ) failed! (ret=%d)\n", ret );
          return ret;
     }

     test->primary->AllowAccess( test->primary, "*" );
     test->primary->GetSize( test->primary, &test->size.w, &test->size.h );
     test->primary->Clear( test->primary, 0, 0, 0, 0 );
     test->primary->Flip( test->primary, NULL, DSFLIP_NONE );

     return DFB_OK;
}

static void ShutdownDFB( DFBContextStruct *test )
{
     if (test->primary)
          test->primary->Release( test->primary );

     if (test->window)
          test->window->Release( test->window );

     if (test->layer)
          test->layer->Release( test->layer );

     if (test->dfb)
          test->dfb->Release( test->dfb );
}

t_ilm_bool createDFBContext(t_ilm_int width, t_ilm_int height)
{
     if (InitDFB( &g_dfbContextStruct, width, height ))
          return ILM_FALSE;

     return ILM_TRUE;
}

#define EGL_CHECK(cmd)                                      \
     if (cmd) {                                             \
          fprintf(stderr, "!!! %s failed\n", #cmd);         \
          goto quit;                                        \
     }

static DFBResult InitEGL( EGLContextStruct *test, int width, int height )
{
     EGLint major, minor, nconfigs;
     EGLint attribs[] = {
          EGL_SURFACE_TYPE,        EGL_WINDOW_BIT,
          EGL_BUFFER_SIZE,         EGL_DONT_CARE,
          EGL_DEPTH_SIZE,          16,
          EGL_RED_SIZE,            0,
          EGL_GREEN_SIZE,          0,
          EGL_BLUE_SIZE,           0,
          EGL_ALPHA_SIZE,          0,
          EGL_RENDERABLE_TYPE,     EGL_OPENGL_ES2_BIT,
          EGL_NONE
     };
     EGLint context_attrs[] = {
          EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
     };
     EGLint surface_attrs[] = {
          EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE
     };
     EGLNativeDisplayType     disp = EGL_DEFAULT_DISPLAY;

     // get display
     EGL_CHECK((test->display = eglGetDisplay(disp)) == EGL_NO_DISPLAY)

     // init
     EGL_CHECK(!eglInitialize(test->display, &major, &minor))

     EGL_CHECK(!eglGetConfigAttribsDIRECTFB(test->display, (EGLNativeWindowType)g_dfbContextStruct.primary, attribs, 0))

     // get configs
     EGL_CHECK(!eglGetConfigs(test->display, test->configs, 2, &nconfigs))

     // choose config
     EGL_CHECK(!eglChooseConfig(test->display, attribs, test->configs, 2, &nconfigs))

     // create a surface
     EGL_CHECK((test->surface = eglCreateWindowSurface(test->display, test->configs[0], (EGLNativeWindowType)g_dfbContextStruct.primary, surface_attrs)) == EGL_NO_SURFACE)

     EGL_CHECK(eglBindAPI(EGL_OPENGL_ES_API) != EGL_TRUE)

     // create context
     EGL_CHECK((test->context = eglCreateContext(test->display, test->configs[0], EGL_NO_CONTEXT, context_attrs)) == EGL_NO_CONTEXT)

     EGL_CHECK(eglMakeCurrent(test->display, test->surface, test->surface, test->context) != EGL_TRUE)

     eglSwapInterval( test->display, 1 );

     /* Setup the viewport */
     glViewport( 0, 0, width, height );

     return DFB_OK;
quit:
     return DFB_FAILURE;
}

t_ilm_bool createEGLContext(t_ilm_int width, t_ilm_int height)
{
     if (InitEGL( &g_eglContextStruct, g_dfbContextStruct.size.w, g_dfbContextStruct.size.h ))
          return ILM_FALSE;

     // register surfaces to layermanager
     t_ilm_layer layerid = (t_ilm_layer)LAYER_EXAMPLE_X_APPLICATIONS;
     t_ilm_surface surfaceid;

     DFBWindowID dfb_wid = 0;
     g_dfbContextStruct.window->GetID(g_dfbContextStruct.window, &dfb_wid);
     surfaceid = dfb_wid;
     ilm_surfaceCreate((t_ilm_nativehandle) dfb_wid, width, height, ILM_PIXELFORMAT_RGBA_8888, &surfaceid);

     ilm_surfaceSetDestinationRectangle(surfaceid, 0, 0, width, height);

     ilm_surfaceSetSourceRectangle(surfaceid, 0, 0, width, height);

     ilm_layerAddSurface(layerid, surfaceid);

     ilm_surfaceSetVisibility(surfaceid, ILM_TRUE);

     ilm_surfaceSetOpacity(surfaceid, 1.0f);

     return(ILM_SUCCESS == ilm_commitChanges()) ? ILM_TRUE : ILM_FALSE;
}

void destroyEglContext()
{
     if (g_eglContextStruct.display != NULL) {
          eglMakeCurrent(g_eglContextStruct.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
          eglTerminate(g_eglContextStruct.display);
     }
}

void destroyDFBContext()
{
     ShutdownDFB( &g_dfbContextStruct );
}

t_ilm_uint GetTickCount()
{
     struct timeval ts;
     gettimeofday(&ts, 0);
     return(t_ilm_uint) (ts.tv_sec * 1000 + (ts.tv_usec / 1000));
}

void swapBuffers()
{
     eglSwapBuffers(g_eglContextStruct.display, g_eglContextStruct.surface);
}

