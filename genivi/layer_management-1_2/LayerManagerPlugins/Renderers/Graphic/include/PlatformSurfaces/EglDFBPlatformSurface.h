/***************************************************************************
 *
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

#ifndef _EGLDFBPLATFORMSURFACE_H_
#define _EGLDFBPLATFORMSURFACE_H_

#include "Surface.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "DFBPlatformSurface.h"

class EglDFBPlatformSurface: public DFBPlatformSurface {
public:
     EglDFBPlatformSurface(Surface* surface, IDirectFBDisplayLayer *layer)
     : DFBPlatformSurface(surface, layer)
     , eglImage(0)
     {
     }

     ~EglDFBPlatformSurface()
     {
     }

     // TODO: private/protected
     EGLImageKHR eglImage;
};


#endif
