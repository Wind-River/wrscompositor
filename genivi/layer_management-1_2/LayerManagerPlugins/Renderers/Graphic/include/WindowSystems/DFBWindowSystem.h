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

#ifndef _DFBWINDOWSYSTEM_H_
#define _DFBWINDOWSYSTEM_H_

#include "GraphicSystems/DFBGraphicSystem.h"
#include "GraphicSystems/GLESGraphicSystem.h"
#include "WindowSystems/BaseWindowSystem.h"
#include "Surface.h"
#include "PlatformSurfaces/EglDFBPlatformSurface.h"
#include "ScreenShotType.h"

extern "C" {
#include <direct/hash.h>
}

class DFBWindowSystem: public BaseWindowSystem {
public:
     DFBWindowSystem(int width, int height, Scene* pScene, InputManager* pInputManager, IDirectFB *dfb, IDirectFBDisplayLayer *layer);
     virtual ~DFBWindowSystem();
     bool init(BaseGraphicSystem<DFBDisplay, DFBWindow>* base, bool dfb_mode_hw);
     bool start(int maxIterationDurationInMS);
     void stop();
     void signalRedrawEvent();
     virtual void allocatePlatformSurface(Surface *surface);
     virtual void deallocatePlatformSurface(Surface *surface);
     void doScreenShot(std::string fileName, const uint screen_id);
     void doScreenShotOfLayer(std::string fileName, const uint id);
     void doScreenShotOfSurface(std::string fileName, const uint id, const uint layer_id);
     DFBDisplay getNativeDisplayHandle();
     int getIterationCounter();
private:
     ScreenShotType takeScreenshot;
     std::string screenShotFile;
     uint screenShotSurfaceID;
     uint screenShotLayerID;
     uint screenShotScreenID;
     int resolutionWidth;
     int resolutionHeight;
     bool m_running;
     bool m_initialized;
     bool m_checkWindows;
     int m_primary_width;
     int m_primary_height;
     int m_mouse_x;
     int m_mouse_y;
     int m_mouse_x_min;
     int m_mouse_y_min;
     int m_mouse_x_max;
     int m_mouse_y_max;
     int m_adjusted_mouse_x;
     int m_adjusted_mouse_y;
     int m_iterationCounter;
     int m_maxIterationDurationInMS;
     std::list<int> *m_sceneLayers;
protected:
     IDirectFB *m_dfb;
     IDirectFBDisplayLayer *m_layer;
     DFBDisplay dfbDisplay;
     IDirectFBSurface *m_primary;
     DirectHash window_map;
     DirectHash surface_map;
     pthread_t renderThread;
     int windowWidth;
     int windowHeight;
     bool m_dfb_mode_hw;
     bool m_ignore_surface_updates;
     DFBSurfaceID CompositorSurface;
     IDirectFBEventBuffer *m_events;
     pthread_mutex_t run_lock;
     pthread_mutex_t init_lock;
     pthread_cond_t init_condition;
     BaseGraphicSystem<DFBDisplay, DFBWindow>* m_gfx;
     virtual void RedrawAllLayers(bool clear, bool swap);
     virtual void renderSWLayers(LayerList layers, bool clear, bool swap);
     virtual void renderHWLayer(Layer* layer, bool clear);
private:
     void cleanup();
     void Screenshot();
     void Redraw(bool clear);
     void createSurfaceForWindow(DFBWindow w);
     void configureSurfaceWindow(DFBWindow w);
     Surface* getSurfaceForWindow(DFBWindow w);
     void checkForNewSurfaceNativeContent();
     void destroy_surface(DFBWindow w);
     void updateSurface(Surface* s, DFBWindow w, DFBPlatformSurface* dfbsurf);
     void MapWindow(DFBWindow w);
     void UnMapWindow(DFBWindow w);
     bool NewWindow(Surface* s, DFBWindow w);
     void DestroyWindow(DFBWindow w);
     bool isWindowValid(DFBWindow w);
     void EventLoop();
     void ManageDFBInputEvent(InputDevice type, InputEventState state, DFBEvent *pevent);
     bool deleteOrphanWindow(void *win);
     int selectOutput(DFBGraphicSystem *gfx, int id);
     friend void *startEventLoop(void *);
     friend bool checkOrphanWindow(DirectHash *hash, unsigned long key, void *value, void *ctx);

     friend class DFBGraphicSystem;
};

#endif

