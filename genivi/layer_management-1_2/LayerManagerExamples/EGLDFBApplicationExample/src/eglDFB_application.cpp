/***************************************************************************
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

#include "ilm_client.h"
#include "egl_helper.h"
#include "gles2application.h"
#include <stdio.h>
#include <unistd.h>

/********************* DEFINES **********************************************/
// Max width and height of the window
#define SURFACE_WIDTH  200
#define SURFACE_HEIGHT 200

#define LAYER_WIDTH    800
#define LAYER_HEIGHT   480
/****************************************************************************/

/********************* TYPEDEFS **********************************************/

/****************************************************************************/

int main(void)
{
     printf("Starting demo EGL DFB Application Example\n");

     if (ilm_init() == ILM_FAILED) {
          printf("Can't Init LayerManagement Communication\n");
          return -1;
     }

     if (!createDFBContext(SURFACE_WIDTH, SURFACE_HEIGHT)) {
          printf("Can't Create DFB Context\n");
          return -1;
     }

     if (!createEGLContext(SURFACE_WIDTH, SURFACE_HEIGHT)) {
          printf("Can't Create EGL Context\n");
          return -1;
     }

     if (!initGlApplication()) {
          printf("Can't Init GL Application\n");
          return -1;
     }

     while (ILM_TRUE) {
          draw(33);
          usleep(1000);
     }

     destroyEglContext();
     destroyDFBContext();
     destroyGlApplication();
     ilm_destroy();
     return 0;
}
