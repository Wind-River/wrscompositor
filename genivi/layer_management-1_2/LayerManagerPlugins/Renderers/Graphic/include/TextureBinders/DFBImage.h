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

#ifndef _DFBIMAGE_H_
#define _DFBIMAGE_H_

#include "TextureBinders/ITextureBinder.h"
#include "PlatformSurfaces/DFBPlatformSurface.h"

class DFBImage: public ITextureBinder {
public:
     virtual PlatformSurface* createPlatformSurface(Surface* surface);

     DFBImage(DFBDisplay dfbDisplay, IDirectFBDisplayLayer *layer);
     bool bindSurfaceTexture(Surface* surface);
     bool unbindSurfaceTexture(Surface* surface);
     void createClientBuffer(Surface* surface);
     void destroyClientBuffer(Surface* surface);

private:
     DFBDisplay m_dfbDisplay;
     IDirectFBDisplayLayer* m_layer;
};

#endif

