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

#include "TextureBinders/DFBImage.h"
#include "PlatformSurfaces/EglDFBPlatformSurface.h"

DFBImage::DFBImage(DFBDisplay dfbDisplay, IDirectFBDisplayLayer* layer)
: m_dfbDisplay(dfbDisplay)
, m_layer(layer)
{
}

bool DFBImage::bindSurfaceTexture(Surface* surface)
{
     (void)surface;

     return true;
}

bool DFBImage::unbindSurfaceTexture(Surface* surface)
{
     (void)surface;

     return true;
}

void DFBImage::createClientBuffer(Surface* surface)
{
     (void)surface;
}

PlatformSurface* DFBImage::createPlatformSurface(Surface* surface)
{
     return new DFBPlatformSurface(surface, m_layer);
}

void DFBImage::destroyClientBuffer(Surface* surface)
{
     (void)surface;
}

