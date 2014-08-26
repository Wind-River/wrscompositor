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

#ifndef _DFBPLATFORMSURFACE_H_
#define _DFBPLATFORMSURFACE_H_

#include "config.h"
#include "Log.h"

#include "PlatformSurface.h"
#include "Surface.h"

#include <directfb.h>

typedef int DFBDisplay;
typedef DFBWindowID DFBWindow;

class DFBPlatformSurface: public PlatformSurface {
public:
     DFBPlatformSurface(Surface* surface, IDirectFBDisplayLayer *layer)
     : PlatformSurface(surface)
     , isMapped(false)
     , dfb_window_id(0)
     , dfb_window(NULL)
     , dfb_surface_id(0)
     , dfb_surface(NULL)
     , texture(0)
     , m_isReadyForRendering(false)
     {
          if (surface->hasNativeContent()) {
               if (layer->GetWindow( layer, (DFBWindowID)surface->getNativeContent(), &dfb_window )) {
                    LOG_ERROR( "DFBPlatformSurface", __FUNCTION__ << ": failed to get IDirectFBWindow by id " << surface->getNativeContent() );
               }
               else {
                    LOG_DEBUG( "DFBPlatformSurface", __FUNCTION__ << ": resolved window id " << surface->getNativeContent() << " to window " << dfb_window );

                    dfb_window_id = surface->getNativeContent();

                    if (dfb_window->GetSurface( dfb_window, &dfb_surface )) {
                         LOG_ERROR( "DFBPlatformSurface", __FUNCTION__ << ": failed to get IDirectFBSurface from window " << dfb_window << " with id " << dfb_window_id );
                    }
                    else {
                         LOG_DEBUG( "DFBPlatformSurface", __FUNCTION__ << ": retrieved surface " << dfb_surface << " from window" );

                         if (dfb_surface->GetID( dfb_surface, &dfb_surface_id )) {
                              LOG_ERROR( "DFBPlatformSurface", __FUNCTION__ << ": failed to get ID from surface " << dfb_surface );
                         }
                         else {
                              LOG_DEBUG( "DFBPlatformSurface", __FUNCTION__ << ": resolved surface " << dfb_surface << " to ID " << dfb_surface_id );
                         }
                    }
               }
          }
     }

     ~DFBPlatformSurface()
     {
          if (dfb_surface)
               dfb_surface->Release( dfb_surface );

          if (dfb_window)
               dfb_window->Release( dfb_window );
     }

     bool enableRendering()
     {
          m_isReadyForRendering = true;
          return m_isReadyForRendering;
     }

     bool disableRendering()
     {
          m_isReadyForRendering = false;
          return m_isReadyForRendering;
     }

     bool isReadyForRendering()
     {
          return m_isReadyForRendering;
     }

     // TODO: private/protected
     bool isMapped;
     DFBWindowID dfb_window_id;
     IDirectFBWindow *dfb_window;
     DFBSurfaceID dfb_surface_id;
     IDirectFBSurface *dfb_surface;
     uint texture;
     bool m_isReadyForRendering;    
};

#endif

