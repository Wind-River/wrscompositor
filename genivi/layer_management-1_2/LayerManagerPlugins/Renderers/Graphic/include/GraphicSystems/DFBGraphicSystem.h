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

#ifndef _DFBGRAPHICSYSTEM_H_
#define _DFBGRAPHICSYSTEM_H_

#include "BaseGraphicSystem.h"
#include "PlatformSurfaces/DFBPlatformSurface.h"

class DFBRenderer;
class DFBWindowSystem;

class DFBGraphicSystem : public BaseGraphicSystem<DFBDisplay, DFBWindow> {
public:
     DFBGraphicSystem(DFBRenderer *renderer, DFBWindowSystem *winsys, IDirectFB *dfb, int windowWidth, int windowHeight);
     virtual ~DFBGraphicSystem();
     virtual bool init(DFBDisplay display, DFBWindow window);

     virtual void beginLayer(Layer* layer);
     virtual void endLayer();

     virtual bool needsRedraw(Layer *layer);
     virtual bool needsRedraw(LayerList layers);
     virtual void renderSWLayer(Layer *layer, bool clear);
     virtual void renderSWLayers(LayerList layers, bool clear);

     virtual void clearBackground();
     virtual void swapBuffers();
     virtual void saveScreenShotOfFramebuffer(std::string fileToSave);
     virtual void renderSurface(Surface* currentSurface);
     virtual void activateGraphicContext();
     virtual void releaseGraphicContext();
private:
     DFBRenderer      *m_renderer;
     DFBWindowSystem  *m_winsys;
     int               m_renderer_output;
     int               m_windowWidth;
     int               m_windowHeight;
     DFBDisplay        m_display;
     DFBWindow         m_window;
     IDirectFB        *m_dfb;
     Layer            *m_currentLayer;
     IDirectFBSurface *m_single_surface;
     int               m_single_x;
     int               m_single_y;

     friend class DFBWindowSystem;
};

#endif

