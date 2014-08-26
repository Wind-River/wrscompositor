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

#include "DFBRenderer.h"
#include "WindowSystems/DFBWindowSystem.h"
#include "config.h"
#include "Layer.h"
#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iomanip>

typedef struct sManagedWindow
{
     DFBPlatformSurface *platform;
     DFBWindowID winID;
     IDirectFBWindow *win;
     DFBSurfaceID surfID;
     IDirectFBSurface *surf;
     int x;
     int y;
     int w;
     int h;
} sManagedWindow;

DFBWindowSystem::DFBWindowSystem(int width, int height, Scene* pScene, InputManager* pInputManager, IDirectFB *dfb, IDirectFBDisplayLayer *layer)
: BaseWindowSystem(pScene, pInputManager)
, takeScreenshot(ScreenShotNone)
, screenShotFile()
, screenShotSurfaceID(0)
, screenShotLayerID()
, screenShotScreenID(0)
, resolutionWidth(width)
, resolutionHeight(height)
, m_running(false)
, m_initialized(false)
, m_checkWindows(false)
, m_primary_width(width)
, m_primary_height(height)
, m_mouse_x(0)
, m_mouse_y(0)
, m_mouse_x_min(-1)
, m_mouse_y_min(-1)
, m_mouse_x_max(-1)
, m_mouse_y_max(-1)
, m_adjusted_mouse_x(0)
, m_adjusted_mouse_y(0)
, m_iterationCounter(0)
, m_maxIterationDurationInMS(0)
, m_dfb(dfb)
, m_layer(layer)
, dfbDisplay(0)
, m_primary(NULL)
, renderThread(0)
, windowWidth(width)
, windowHeight(height)
, m_dfb_mode_hw(true)
, m_ignore_surface_updates(false)
, CompositorSurface(0)
, m_events(NULL)
, m_gfx(NULL)
{
     pthread_mutex_init( &run_lock, NULL );
     pthread_mutex_init( &init_lock, NULL );
     pthread_cond_init( &init_condition,NULL );
     direct_hash_init( &window_map, 17 );
     direct_hash_init( &surface_map, 17 );
     m_sceneLayers = new std::list<int>;
}

DFBWindowSystem::~DFBWindowSystem()
{
     pthread_mutex_destroy( &run_lock );
     pthread_mutex_destroy( &init_lock );
     pthread_cond_destroy( &init_condition );
     direct_hash_deinit( &window_map );
     direct_hash_deinit( &surface_map );
}

bool DFBWindowSystem::isWindowValid(DFBWindow w)
{
     sManagedWindow *win = (sManagedWindow *)direct_hash_lookup( &window_map, w );
     if (!win)
          return true;

     return (w != 0 && win->surfID != CompositorSurface);
}

Surface* DFBWindowSystem::getSurfaceForWindow(DFBWindow w)
{
     const std::map<unsigned int,Surface*> surfaces = m_pScene->getAllSurfaces();
     for (std::map<unsigned int, Surface*>::const_iterator currentS = surfaces.begin(); currentS != surfaces.end(); ++currentS) {
          Surface* currentSurface = (*currentS).second;
          if (!currentSurface)
               continue;

          if (currentSurface->getNativeContent() == static_cast<long>(w))
               return currentSurface;
     }

     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": could not get surface for window " << w );

     return NULL;
}

bool DFBWindowSystem::deleteOrphanWindow(void *win)
{
     sManagedWindow *managed = (sManagedWindow *)win;

     if (!getSurfaceForWindow( managed->winID )) {
          LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": removing orphan window " << managed->winID );

          direct_hash_remove( &window_map, managed->winID );
          direct_hash_remove( &surface_map, managed->surfID );

          managed->win->DetachEventBuffer( managed->win, m_events );
          managed->surf->DetachEventBuffer( managed->surf, m_events );

          delete managed->platform;

          return true;
     }

     return false;
}

bool checkOrphanWindow(DirectHash *hash, unsigned long key, void *value, void *ctx)
{
     (void)hash;
     (void)key;
     DFBWindowSystem *windowsys = static_cast<DFBWindowSystem *>((DFBWindowSystem *)ctx);

     windowsys->deleteOrphanWindow( value );

     return true;
}

void DFBWindowSystem::checkForNewSurfaceNativeContent()
{
     m_pScene->lockScene();

     LayerList layers = m_pScene->getCurrentRenderOrder(0);
     for (LayerListConstIterator current = layers.begin(); current != layers.end(); current++) {
          SurfaceList surfaces = (*current)->getAllSurfaces();
          for (SurfaceListConstIterator currentS = surfaces.begin(); currentS != surfaces.end(); currentS++) {
               if ((*currentS)->getNativeContent())
                    allocatePlatformSurface( *currentS );
               else
                    deallocatePlatformSurface( *currentS );
          }
     }

     if (direct_hash_count( &window_map ))
          direct_hash_iterate( &window_map, checkOrphanWindow, this );

     m_pScene->unlockScene();

     m_checkWindows = false;
}

void DFBWindowSystem::configureSurfaceWindow(DFBWindow window)
{
     if (isWindowValid( window )) {
          int winWidth = 0;
          int winHeight = 0;

          UnMapWindow( window );
          MapWindow( window );

          Surface* surface = getSurfaceForWindow( window );
          if (!surface) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find surface for window " << window );
               return;
          }

          if (!surface->platform) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": Platform surface not available for window " << window );
               return;
          }

          DFBPlatformSurface* platformSurface = (DFBPlatformSurface*)surface->platform;
          platformSurface->dfb_surface->GetSize( platformSurface->dfb_surface, &winWidth, &winHeight );

          surface->OriginalSourceHeight = winHeight;
          surface->OriginalSourceWidth = winWidth;

          LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": successfully updated window " << window );
     }
}

void DFBWindowSystem::MapWindow(DFBWindow window)
{
     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(window=" << window << ")" );

     if (isWindowValid( window )) {
          Surface* surface = getSurfaceForWindow( window );
          if (!surface) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find surface for window " << window );
               return;
          }

          if (!surface->platform) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": Platform surface not available for window " << window );
               return;
          }

          DFBPlatformSurface* dfbsurf = (DFBPlatformSurface*)surface->platform;
          if (dfbsurf->isMapped) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": Platform surface for window " << window << " was already mapped" );
               return;
          }

          dfbsurf->isMapped = true;

          sManagedWindow *win = (sManagedWindow *)malloc( sizeof(sManagedWindow) );
          win->platform = dfbsurf;
          win->winID = dfbsurf->dfb_window_id;
          win->win = dfbsurf->dfb_window;
          win->surfID = dfbsurf->dfb_surface_id;
          win->surf = dfbsurf->dfb_surface;
          dfbsurf->dfb_window->GetPosition( dfbsurf->dfb_window, &win->x, &win->y );
          dfbsurf->dfb_window->GetSize( dfbsurf->dfb_window, &win->w, &win->h );

          direct_hash_insert( &window_map, dfbsurf->dfb_window_id, win );
          direct_hash_insert( &surface_map, dfbsurf->dfb_surface_id, win );

          surface->OriginalSourceWidth = win->w;
          surface->OriginalSourceHeight = win->h;
          surface->renderPropertyChanged = true;

          m_gfx->getTextureBinder()->createClientBuffer( surface );
          dfbsurf->enableRendering();

          LOG_INFO( "DFBWindowSystem", __FUNCTION__ << ": successfully mapped window " << window );
     }
}

void DFBWindowSystem::UnMapWindow(DFBWindow window)
{
     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(window=" << window << ")" );

     if (isWindowValid( window )) {
          Surface* surface = getSurfaceForWindow( window );        
          if (!surface) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find surface for window " << window );
               return;
          }

          if (!surface->platform) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": Platform surface not available for window " << window );
               return;
          }

          DFBPlatformSurface* dfbsurf = (DFBPlatformSurface*)surface->platform;
          dfbsurf->disableRendering();
          if (!dfbsurf->isMapped) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": Platform surface for window " << window << " was already unmapped" );
               return;
          }

          dfbsurf->isMapped = false;
          m_gfx->getTextureBinder()->destroyClientBuffer( surface );

          if (dfbsurf->dfb_surface) {
               dfbsurf->dfb_window->DetachEventBuffer( dfbsurf->dfb_window, m_events );
               dfbsurf->dfb_surface->DetachEventBuffer( dfbsurf->dfb_surface, m_events );
               direct_hash_remove( &window_map, dfbsurf->dfb_window_id );
               direct_hash_remove( &surface_map, dfbsurf->dfb_surface_id );
               dfbsurf->dfb_surface->Release( dfbsurf->dfb_surface );
               dfbsurf->dfb_window->Release( dfbsurf->dfb_window );
          }

          surface->renderPropertyChanged = true;

          LOG_INFO( "DFBWindowSystem", __FUNCTION__ << ": successfully unmapped window " << window );
     }
}

bool DFBWindowSystem::NewWindow(Surface* surface, DFBWindow window)
{
     DFBResult ret;
     int winWidth = 0;
     int winHeight = 0;

     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(surface=" << surface << ", window=" << window << ")" );

     if (isWindowValid( window )) {
          surface->setNativeContent( window );
          DFBPlatformSurface *platformSurface = (DFBPlatformSurface*)m_gfx->getTextureBinder()->createPlatformSurface( surface );
          platformSurface->isMapped = false;

          if (!platformSurface->dfb_surface) {
               LOG_ERROR( "DFBWindowSystem", __FUNCTION__ << ": failed to create platform surface for window " << window );
               delete platformSurface;
               return false;
          }

          surface->platform = platformSurface;

          platformSurface->dfb_window->GetSize( platformSurface->dfb_window, &winWidth, &winHeight );

          ret = platformSurface->dfb_window->AttachEventBuffer( platformSurface->dfb_window, m_events );
          if (ret) {
               LOG_ERROR( "DFBWindowSystem", __FUNCTION__ << ": IDirectFBSurface::AttachEventBuffer() failed! (ret=" << ret << ")" );
               delete platformSurface;
               surface->platform = NULL;
               return false;
          }

          ret = platformSurface->dfb_surface->AttachEventBuffer( platformSurface->dfb_surface, m_events );
          if (ret) {
               LOG_ERROR( "DFBWindowSystem", __FUNCTION__ << ": IDirectFBSurface::AttachEventBuffer() failed! (ret=" << ret << ")" );
               delete platformSurface;
               surface->platform = NULL;
               return false;
          }

          platformSurface->dfb_surface->MakeClient( platformSurface->dfb_surface );
          surface->OriginalSourceHeight = winHeight;
          surface->OriginalSourceWidth = winWidth;

          LOG_INFO( "DFBWindowSystem", __FUNCTION__ << ": successfully created new surface " << platformSurface->dfb_surface << " for window " << window << " (w=" << winWidth << " h=" << winHeight << ")" );

          return true;
     }

     return false;
}

void DFBWindowSystem::DestroyWindow(DFBWindow window)
{
     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(window=" << window << ")" );

     if (isWindowValid( window )) {
          Surface* surface = getSurfaceForWindow( window );
          if (!surface) {
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find surface for window " << window );
               return;
          }

          m_gfx->getTextureBinder()->destroyClientBuffer( surface );
          UnMapWindow( window );
          surface->removeNativeContent();

          surface->renderPropertyChanged = true;
          delete surface->platform;
          surface->platform = NULL;
     }
}

int DFBWindowSystem::selectOutput(DFBGraphicSystem *gfx, int id)
{
     int ret = -1;

     for (int i = 0; i < gfx->m_renderer->m_num_outputs; i++) {
          if (gfx->m_renderer->m_outputs[i].layer_id == id) {
               ret = i;
               break;
          }
     }

     return ret;
}

void DFBWindowSystem::RedrawAllLayers(bool clear, bool swap)
{
     DFBGraphicSystem *gfx;
     DFBDisplayLayerConfig config;

     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(clear=" << clear << ", swap=" << swap << ")" );

     LayerList layers = m_pScene->getCurrentRenderOrder( 0 );
     LayerList swLayers;
     std::list<int> orphan_ids = list<int>( *m_sceneLayers );

     m_sceneLayers->clear();

     for (LayerListConstIterator current = layers.begin(); current != layers.end(); current++) {
          m_sceneLayers->push_back( (*current)->getID() );
          orphan_ids.remove( (*current)->getID() );

          if ((*current)->getLayerType() == Hardware) {
               if (m_forceComposition || m_gfx->needsRedraw( *current ))
                    renderHWLayer( *current, clear );
          }
          else
               swLayers.push_back( *current );
     }

     if (m_dfb_mode_hw) {
          gfx = (DFBGraphicSystem *)m_gfx;

          for(std::list<int>::iterator list_iter = orphan_ids.begin(); list_iter != orphan_ids.end(); list_iter++) {
               int output = selectOutput( gfx, *list_iter );
               if (output >= 0) {
                    if (!gfx->m_renderer->m_outputs[output].muted) {
                         LOG_INFO( "DFBWindowSystem", __FUNCTION__ << ": turning OFF orphan layer " << gfx->m_renderer->m_outputs[output].layer );

                         if (!gfx->m_renderer->m_outputs[output].layer->GetConfiguration( gfx->m_renderer->m_outputs[output].layer, &config )) {
                              config.options = DLOP_OPACITY;

                              if (gfx->m_renderer->m_outputs[output].layer->SetConfiguration( gfx->m_renderer->m_outputs[output].layer, &config )) {
                                   LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": failed to set layer configuration on layer " << gfx->m_renderer->m_outputs[output].layer );
                              }
                              else {
                                   if (gfx->m_renderer->m_outputs[output].layer->SetOpacity( gfx->m_renderer->m_outputs[output].layer, 0 )) {
                                        LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": failed to set opacity on layer " << gfx->m_renderer->m_outputs[output].layer );
                                   }
                                   else {
                                        gfx->m_renderer->m_outputs[output].muted = true;
                                        gfx->m_renderer->m_outputs[output].layer_opacity = 0;
                                   }
                              }
                         }
                    }
               }
          }
     }

     if (swLayers.size() > 0 && (m_forceComposition || m_gfx->needsRedraw( swLayers ))) {
          if (m_dfb_mode_hw)
               renderSWLayers( swLayers, clear, swap );
          else {
               m_gfx->renderSWLayers( swLayers, clear );

               if (swap)
                    m_gfx->swapBuffers();
          }
     }
}

void DFBWindowSystem::renderSWLayers(LayerList layers, bool clear, bool swap)
{
     DFBGraphicSystem *gfx = (DFBGraphicSystem *)m_gfx;

     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(clear=" << clear << ", swap=" << swap << ")" );

     for (LayerListConstIterator layer = layers.begin(); layer != layers.end(); layer++) {
          gfx->renderSWLayer( *layer, clear );
          if (swap) {
               gfx->beginLayer( *layer );
               if (gfx->m_renderer_output == -1) {
                    LOG_ERROR( "DFBWindowSystem", __FUNCTION__ << ": called with a non-existent layer" );
                    gfx->endLayer();
                    continue;
               }
               gfx->swapBuffers();
               gfx->endLayer();
          }
     }
}

static int countVisibleSurfaces(Layer *layer)
{
     int ret = 0;
     FloatRectangle rect;

     SurfaceList surfaces = layer->getAllSurfaces();
     for (SurfaceListConstIterator currentS = surfaces.begin(); currentS != surfaces.end(); currentS++) {
          if ((*currentS)->getOpacity() > 0 && (*currentS)->getVisibility()) {
               rect = (*currentS)->getTargetSourceRegion();
               if (rect.width > 0 && rect.height > 0) {
                    rect = (*currentS)->getTargetDestinationRegion();
                    if (rect.width > 0 && rect.height > 0)
                         ret++;
               }
          }
     }

     return ret;
}

static bool isSurfaceVisible(Surface *surface)
{
     FloatRectangle rect;

     if (surface->getOpacity() > 0 && surface->getVisibility()) {
          rect = surface->getTargetSourceRegion();
          if (rect.width > 0 && rect.height > 0) {
               rect = surface->getTargetDestinationRegion();
               if (rect.width > 0 && rect.height > 0)
                    return true;
          }
     }

     return false;
}

void DFBWindowSystem::renderHWLayer(Layer *layer, bool clear)
{
     DFBGraphicSystem *gfx = (DFBGraphicSystem *)m_gfx;
     DFBDisplayLayerConfig config;
     int num_surfaces_curr = 0;

     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(layer=" << layer << ", clear=" << clear << ")" );

     gfx->beginLayer( layer );
     if (gfx->m_renderer_output == -1) {
          LOG_ERROR( "DFBWindowSystem", __FUNCTION__ << ": called with a non-existent layer" );

          gfx->endLayer();
          return;
     }

     if (layer->getLayerType() != Hardware)
          LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": called with a non-HW layer" );

     if (!m_pScene->isLayerInCurrentRenderOrder( layer->getID() ) || layer->opacity == 0 || layer->visibility == 0) {
          if (!gfx->m_renderer->m_outputs[gfx->m_renderer_output].muted) {
               LOG_INFO( "DFBWindowSystem", __FUNCTION__ << ": turning OFF layer " << layer);

               if (!gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->GetConfiguration( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, &config )) {
                    config.options = DLOP_OPACITY;

                    if (gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->SetConfiguration( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, &config )) {
                         LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": failed to set layer configuration on layer " << gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer );
                    }
                    else {
                         if (gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->SetOpacity( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, 0 )) {
                              LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": failed to set opacity on layer " << gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer );
                         }
                         else {
                              gfx->m_renderer->m_outputs[gfx->m_renderer_output].muted = true;
                              gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer_opacity = 0;
                         }
                    }
               }
          }
     }
     else if (layer->visibility > 0 && layer->opacity > 0.0f) {
          num_surfaces_curr = countVisibleSurfaces( layer );
          if (num_surfaces_curr > 0) {
               if (layer->getChromaKeyEnabled())
                    LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": cannot handle chroma key" );

               if (clear)
                    gfx->clearBackground();

               if (gfx->m_renderer->m_outputs[gfx->m_renderer_output].muted || gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer_opacity != layer->opacity) {
                    LOG_INFO( "DFBWindowSystem", __FUNCTION__ << ": turning ON layer " << layer << " with opacity=" << (int)(layer->opacity * 0xFF) );

                    if (!gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->GetConfiguration( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, &config )) {
                         config.options = layer->opacity == 1 ? DLOP_NONE : (DFBDisplayLayerOptions)(DLOP_OPACITY | DLOP_ALPHACHANNEL);

                         if (gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->SetConfiguration( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, &config )) {
                              LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": failed to set layer configuration on layer " << gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer );
                         }
                         else {
                              if (gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->SetOpacity( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, layer->opacity * 0xFF )) {
                                   LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": failed to set opacity on layer " << gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer );
                              }
                              else {
                                   gfx->m_renderer->m_outputs[gfx->m_renderer_output].muted = false;
                                   gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer_opacity = layer->opacity;
                              }
                         }
                    }
               }

               SurfaceList surfaces = layer->getAllSurfaces();
               for (std::list<Surface*>::const_iterator currentS = surfaces.begin(); currentS != surfaces.end(); ++currentS) {
                    if ((*currentS)->hasNativeContent() && isSurfaceVisible( (*currentS) ))
                         gfx->renderSurface( *currentS );
               }
          }
          else if (!gfx->m_renderer->m_outputs[gfx->m_renderer_output].muted) {
               LOG_INFO( "DFBWindowSystem", __FUNCTION__ << ": tunring OFF layer " << layer);

               if (!gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->GetConfiguration( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, &config )) {
                    config.options = DLOP_OPACITY;

                    if (gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->SetConfiguration( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, &config )) {
                         LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": failed to set layer configuration on layer " << gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer );
                    }
                    else {
                         if (gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer->SetOpacity( gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer, 0 )) {
                              LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": failed to set opacity on layer " << gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer );
                         }
                         else {
                              gfx->m_renderer->m_outputs[gfx->m_renderer_output].muted = true;
                              gfx->m_renderer->m_outputs[gfx->m_renderer_output].layer_opacity = 0;
                         }
                    }
               }
          }

          gfx->m_renderer->m_outputs[gfx->m_renderer_output].num_surfaces = num_surfaces_curr;
     }

     gfx->swapBuffers();

     gfx->endLayer();
}

void DFBWindowSystem::Redraw(bool clear)
{
     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "()" );

     m_pScene->lockScene();

     RedrawAllLayers( clear, true );
     ClearDamage();

     m_pScene->unlockScene();
}

void DFBWindowSystem::Screenshot()
{
     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "()" );

     m_pScene->lockScene();

     if (takeScreenshot == ScreenshotOfDisplay) {
          if (!m_dfb_mode_hw) {
               RedrawAllLayers( true, false );
               m_gfx->saveScreenShotOfFramebuffer( screenShotFile );
          }
          else
               LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": Screetshot of FrameBuffer currently unsupported!" );
     }
     else if (takeScreenshot == ScreenshotOfLayer) {
          Layer* layer = m_pScene->getLayer( screenShotLayerID );
          if (layer != NULL) {
               m_gfx->renderSWLayer( layer, true );
               if (m_dfb_mode_hw)
                    m_gfx->beginLayer( layer );
               m_gfx->saveScreenShotOfFramebuffer( screenShotFile );
               if (m_dfb_mode_hw)
                    m_gfx->endLayer();
          }
     }
     else if (takeScreenshot == ScreenshotOfSurface) {
          Layer* layer = m_pScene->getLayer( screenShotLayerID );
          Surface* surface = m_pScene->getSurface( screenShotSurfaceID );
          if (layer != NULL && surface != NULL) {
               m_gfx->clearBackground();
               m_gfx->beginLayer( layer );
               m_gfx->renderSurface( surface );
               m_gfx->saveScreenShotOfFramebuffer( screenShotFile );
               m_gfx->endLayer();
          }
     }

     takeScreenshot = ScreenShotNone;

     m_pScene->unlockScene();
}

void *startEventLoop(void *ptr)
{
     DFBWindowSystem *windowsys = static_cast<DFBWindowSystem *>((DFBWindowSystem *)ptr);
     windowsys->EventLoop();

     return NULL;
}

static void genWindowEventFromInput(const DFBInputEvent *inputEvent, DFBWindowID wid, DFBWindowEventType etype, int x, int y, int cx, int cy, DFBWindowEvent *windowEvent)
{
     memset( windowEvent, 0, sizeof(DFBWindowEvent) );

     windowEvent->type = etype;
     windowEvent->window_id = wid; 
     windowEvent->flags = DWEF_DEVICE_ID;
     windowEvent->x = x;
     windowEvent->y = y;
     windowEvent->cx = cx;
     windowEvent->cy = cy;
     windowEvent->device_id = inputEvent->device_id;
     windowEvent->timestamp = inputEvent->timestamp;
     windowEvent->key_code = inputEvent->key_code;
     windowEvent->key_id = inputEvent->key_id;
     windowEvent->key_symbol = inputEvent->key_symbol;
     windowEvent->modifiers = inputEvent->modifiers;
     windowEvent->locks = inputEvent->locks;
     windowEvent->button = inputEvent->button;
     windowEvent->buttons = inputEvent->buttons;
}

void DFBWindowSystem::EventLoop()
{
     Surface *surface;
     sManagedWindow *win;
     DFBPlatformSurface *surf;
     DFBWindowEvent windowEvent;
     DFBEvent event;
     bool redraw = false;

     if (m_primary) {
          LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": using compositor primary " << m_primary << " with id " << CompositorSurface << " (w=" << m_primary_width << " h=" << m_primary_height << ")" );

          m_primary->Clear( m_primary, 0, 0, 0, 0 );
          m_primary->Flip( m_primary, NULL, DSFLIP_NONE );
     
          m_gfx->init( dfbDisplay, (long)m_primary );
          m_gfx->activateGraphicContext();
          m_gfx->clearBackground();
          m_gfx->swapBuffers();
     }

     pthread_mutex_lock( &init_lock );
     m_running = true;
     pthread_cond_signal( &init_condition );
     pthread_mutex_unlock( &init_lock );

     while (m_running) {
          if (m_checkWindows) {
               checkForNewSurfaceNativeContent();
               Redraw( true );
               redraw = false;
          }
          else if (redraw) {
               Redraw( true );
               redraw = false;
          }

          if (takeScreenshot != ScreenShotNone)
               Screenshot();

          m_events->WaitForEventWithTimeout( m_events, 0, 300 );

          pthread_mutex_lock( &run_lock );

          while (m_events->GetEvent( m_events, &event ) == DFB_OK) {
               switch (event.clazz) {
                    case DFEC_WINDOW:
                         switch (event.window.type) {
                              case DWET_POSITION:
                                   LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": WINDOW POSITION event for window id " << event.window.window_id );

                                   win = (sManagedWindow *)direct_hash_lookup( &window_map, event.window.window_id );
                                   if (win) {
                                        win->x = event.window.x;
                                        win->y = event.window.y;
                                   }
                                   else
                                        LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find window for id " << event.window.window_id );
                                   break;
                              default:
                                   break;
                         }
                         break;
                    case DFEC_SURFACE:
                         switch (event.surface.type) {
                              case DSEVT_UPDATE:
                                   if (!m_ignore_surface_updates) {
                                        LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": SURFACE UPDATE event for surface id " << event.surface.surface_id );

                                        win = (sManagedWindow *)direct_hash_lookup( &surface_map, event.surface.surface_id );
                                        if (win) {
                                             LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": sending FrameAck for surface id " << event.surface.surface_id << " (frame=" << event.surface.flip_count << ")" );
                                             win->surf->FrameAck( win->surf, event.surface.flip_count );
                                             redraw = true;
                                        }
                                        else
                                             LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find surface for id " << event.surface.surface_id );
                                   }
                                   else
                                        LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": SURFACE UPDATE event for surface id " << event.surface.surface_id << " -- ignoring" );
                                   break;
                              default:
                                   break;
                         }
                         break;
                    case DFEC_INPUT:
                         Point pos;
                         pos.x = m_adjusted_mouse_x;
                         pos.y = m_adjusted_mouse_y;
                         switch (event.input.type) {
                              case DIET_KEYPRESS:
                                   LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << " --> KEYPRESS" );

                                   surface = m_pInputManager->reportKeyboardEvent( INPUT_STATE_PRESSED, event.input.key_code );
                                   if (surface && surface->platform) {
                                        surf = (DFBPlatformSurface *)surface->platform;
                                        win = (sManagedWindow *)direct_hash_lookup( &window_map, event.window.window_id );
                                        if (!win) {
                                             LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find window for id " << event.window.window_id );
                                             break;
                                        }
                                        genWindowEventFromInput( &event.input, surf->dfb_window_id, DWET_KEYDOWN, m_adjusted_mouse_x, m_adjusted_mouse_y, m_adjusted_mouse_x - win->x, m_adjusted_mouse_y - win->y, &windowEvent );
                                        win->win->SendEvent( win->win, &windowEvent );
                                   }
                                   break;
                              case DIET_KEYRELEASE:
                                   LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << " --> KEYRELEASE" );

                                   surface = m_pInputManager->reportKeyboardEvent( INPUT_STATE_RELEASED, event.input.key_code );
                                   if (surface && surface->platform) {
                                        surf = (DFBPlatformSurface *)surface->platform;
                                        win = (sManagedWindow *)direct_hash_lookup( &window_map, event.window.window_id );
                                        if (!win) {
                                             LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find window for id " << event.window.window_id );
                                             break;
                                        }
                                        genWindowEventFromInput( &event.input, surf->dfb_window_id, DWET_KEYUP, m_adjusted_mouse_x, m_adjusted_mouse_y, m_adjusted_mouse_x - win->x, m_adjusted_mouse_y - win->y, &windowEvent );
                                        win->win->SendEvent( win->win, &windowEvent );
                                   }
                                   break;
                              case DIET_BUTTONPRESS:
                                   LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << " --> BUTTONPRESS" );

                                   pos.state = INPUT_STATE_PRESSED;
                                   surface = m_pInputManager->reportPointerEvent( pos );
                                   if (surface && surface->platform) {
                                        surf = (DFBPlatformSurface *)surface->platform;
                                        win = (sManagedWindow *)direct_hash_lookup( &window_map, event.window.window_id );
                                        if (!win) {
                                             LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find window for id " << event.window.window_id );
                                             break;
                                        }
                                        genWindowEventFromInput( &event.input, surf->dfb_window_id, DWET_BUTTONDOWN, m_adjusted_mouse_x, m_adjusted_mouse_y, m_adjusted_mouse_x - win->x, m_adjusted_mouse_y - win->y, &windowEvent );
                                        win->win->SendEvent( win->win, &windowEvent );
                                   }
                                   break;
                              case DIET_BUTTONRELEASE:
                                   LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << " --> BUTTONRELEASE" );

                                   pos.state = INPUT_STATE_RELEASED;
                                   surface = m_pInputManager->reportPointerEvent( pos );
                                   if (surface && surface->platform) {
                                        surf = (DFBPlatformSurface *)surface->platform;
                                        win = (sManagedWindow *)direct_hash_lookup( &window_map, event.window.window_id );
                                        if (!win) {
                                             LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find window for id " << event.window.window_id );
                                             break;
                                        }
                                        genWindowEventFromInput( &event.input, surf->dfb_window_id, DWET_BUTTONUP, m_adjusted_mouse_x, m_adjusted_mouse_y, m_adjusted_mouse_x - win->x, m_adjusted_mouse_y - win->y, &windowEvent );
                                        win->win->SendEvent( win->win, &windowEvent );
                                   }
                                   break;
                              case DIET_AXISMOTION:
                                   pos.state = INPUT_STATE_MOTION;
                                   if (event.input.flags & DIEF_AXISABS) {
                                        switch (event.input.axis) {
                                             case DIAI_X:
                                                  m_mouse_x = event.input.axisabs;
                                                  m_mouse_x_min = event.input.min;
                                                  m_mouse_x_max = event.input.max;
                                                  break;
                                             case DIAI_Y:
                                                  m_mouse_y = event.input.axisabs;
                                                  m_mouse_y_min = event.input.min;
                                                  m_mouse_y_max = event.input.max;
                                                  break;
                                             default:
                                                  break;
                                        }
                                   }
                                   else if (event.input.flags & DIEF_AXISREL) {
                                        switch (event.input.axis) {
                                             case DIAI_X:
                                                  m_mouse_x += event.input.axisrel;
                                                  break;
                                             case DIAI_Y:
                                                  m_mouse_y += event.input.axisrel;
                                                  break;
                                             default:
                                                  break;
                                        }
                                   }

                                   if (m_mouse_y_min < m_mouse_y_max && m_mouse_x_min < m_mouse_x_max) {
                                        m_adjusted_mouse_x = CLAMP(m_mouse_x, 0, m_mouse_x_max);
                                        m_adjusted_mouse_y = CLAMP(m_mouse_y, 0, m_mouse_y_max);
                                        m_adjusted_mouse_x = ((m_primary_width - 1) * m_adjusted_mouse_x) / m_mouse_x_max;
                                        m_adjusted_mouse_y = ((m_primary_height - 1) * m_adjusted_mouse_y) / m_mouse_y_max;
                                   }
                                   else {
                                        m_adjusted_mouse_x = CLAMP(m_mouse_x, 0, m_primary_width - 1);
                                        m_adjusted_mouse_y = CLAMP(m_mouse_y, 0, m_primary_height - 1);
                                   }

                                   LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << " --> MOTION at " << m_adjusted_mouse_x << "," << m_adjusted_mouse_y );

                                   pos.x = m_adjusted_mouse_x;
                                   pos.y = m_adjusted_mouse_y;

                                   surface = m_pInputManager->reportPointerEvent( pos );
                                   if (surface && surface->platform) {
                                        surf = (DFBPlatformSurface *)surface->platform;
                                        win = (sManagedWindow *)direct_hash_lookup( &window_map, event.window.window_id );
                                        if (!win) {
                                             LOG_WARNING( "DFBWindowSystem", __FUNCTION__ << ": could not find window for id " << event.window.window_id );
                                             break;
                                        }
                                        genWindowEventFromInput( &event.input, surf->dfb_window_id, DWET_MOTION, m_adjusted_mouse_x, m_adjusted_mouse_y, m_adjusted_mouse_x - win->x, m_adjusted_mouse_y - win->y, &windowEvent );
                                        win->win->SendEvent( win->win, &windowEvent );
                                   }
                                   break;
                              default:
                                   LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << " --> UNKNOWN INPUT EVENT" );
                                   break;
                         }
                         break;
                    default:
                         break;
               }
          }

          pthread_mutex_unlock( &run_lock );
     }

     cleanup();

     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << " --> finished" );
}

void DFBWindowSystem::signalRedrawEvent()
{
     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "()" );

     pthread_mutex_lock( &run_lock );

     m_checkWindows = true;
     m_events->WakeUp( m_events );

     pthread_mutex_unlock( &run_lock );
}

void DFBWindowSystem::cleanup()
{
     pthread_mutex_lock( &run_lock );

     m_running = false;
     m_initialized = false;

     if (m_events) {
          m_events->Release( m_events );
          m_events = NULL;
     }

     if (m_primary) {
          m_primary->Release( m_primary );
          m_primary = NULL;
     }

     pthread_mutex_unlock( &run_lock );
}

bool DFBWindowSystem::init(BaseGraphicSystem<DFBDisplay, DFBWindow>* base, bool dfb_mode_hw)
{
     DFBResult ret;

     LOG_INFO( "DFBWindowSystem", __FUNCTION__ << "(hw=" << dfb_mode_hw << ")" );

     m_gfx = base;
     m_dfb_mode_hw = dfb_mode_hw;

     m_forceComposition = true;

     ret = m_dfb->CreateInputEventBuffer( m_dfb, (DFBInputDeviceCapabilities)(DICAPS_KEYS | DICAPS_BUTTONS | DICAPS_AXES), DFB_FALSE, &m_events );
     if (ret) {
          LOG_ERROR( "DFBWindowSystem", __FUNCTION__ << ": IDirectFB::CreateEventBuffer() failed! (ret=" << ret << ")" );
          return false;
     }

     if (!m_dfb_mode_hw) {
          DFBSurfaceID surf_id;

          ret = m_layer->GetSurface( m_layer, &m_primary );
          if (ret) {
               LOG_ERROR( "DFBWindowSystem", __FUNCTION__ << ": IDirectFBDisplayLayer::GetSurface() failed! (ret=" << ret << ")" );
               m_events->Release( m_events );
               if (m_primary)
                    m_primary->Release( m_primary );
     
               return false;
          }
     
          m_primary->GetSize( m_primary, &m_primary_width, &m_primary_height );
     
          LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << ": retrieved layer primary " << m_primary << " (w=" << m_primary_width << " h=" << m_primary_height << ")" );
     
          m_primary->GetID( m_primary, &surf_id );
          CompositorSurface = surf_id;
     }

     pthread_mutex_lock( &init_lock );

     int status = pthread_create( &renderThread, NULL, startEventLoop, this );
     if (status) {
          pthread_mutex_unlock( &init_lock );
          return false;
     }

     if (!m_running)
          pthread_cond_wait( &init_condition, &init_lock );

     pthread_mutex_unlock( &init_lock );

     LOG_INFO( "DFBWindowSystem", __FUNCTION__ << " --> initialized" );

     return true;
}

bool DFBWindowSystem::start(int maxIterationDurationInMS)
{
     LOG_INFO( "DFBWindowSystem", __FUNCTION__ << "(" << maxIterationDurationInMS << ")" );

     pthread_mutex_lock( &run_lock );

     LayerList layers = m_pScene->getCurrentRenderOrder( 0 );
     for (LayerListConstIterator current = layers.begin(); current != layers.end(); current++)
          m_sceneLayers->push_back( (*current)->getID() );

     m_maxIterationDurationInMS = maxIterationDurationInMS;

     m_running = true;

     m_checkWindows = true;
     m_events->WakeUp( m_events );

     pthread_mutex_unlock( &run_lock );

     return true;
}

void DFBWindowSystem::stop()
{
     LOG_INFO( "DFBWindowSystem", __FUNCTION__ << "()" );

     pthread_mutex_lock( &run_lock );

     m_sceneLayers->clear();

     m_running = false;
     m_events->WakeUp( m_events );

     pthread_mutex_unlock( &run_lock );

     pthread_join( renderThread, NULL );
}

void DFBWindowSystem::allocatePlatformSurface(Surface* surface)
{
     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(" << surface << ")" );

     DFBPlatformSurface* nativeSurface = (DFBPlatformSurface*)surface->platform;
     if (!nativeSurface) {
          if (NewWindow( surface, surface->getNativeContent() ))
               MapWindow( surface->getNativeContent() );
     }
}

void DFBWindowSystem::deallocatePlatformSurface(Surface* surface)
{
     LOG_DEBUG( "DFBWindowSystem", __FUNCTION__ << "(" << surface << ")" );

     DFBPlatformSurface* nativeSurface = (DFBPlatformSurface*)surface->platform;
     if (nativeSurface) {
          m_gfx->getTextureBinder()->destroyClientBuffer( surface );

          if (nativeSurface->dfb_surface)
               nativeSurface->dfb_surface->Release( nativeSurface->dfb_surface );

          if (nativeSurface->dfb_window)
               nativeSurface->dfb_window->Release( nativeSurface->dfb_window );

          surface->renderPropertyChanged = true;
          delete surface->platform;
          surface->platform = NULL;
     }
}

void DFBWindowSystem::doScreenShot(std::string fileName, const uint screen_id)
{
     takeScreenshot = ScreenshotOfDisplay;
     screenShotFile = fileName;
     screenShotScreenID = screen_id;
}

void DFBWindowSystem::doScreenShotOfLayer(std::string fileName, const uint id)
{
     takeScreenshot = ScreenshotOfLayer;
     screenShotFile = fileName;
     screenShotLayerID = id;
}

void DFBWindowSystem::doScreenShotOfSurface(std::string fileName, const uint id, const uint layer_id)
{
     takeScreenshot = ScreenshotOfSurface;
     screenShotFile = fileName;
     screenShotSurfaceID = id;
     screenShotLayerID = layer_id;
}

DFBDisplay DFBWindowSystem::getNativeDisplayHandle()
{
     return dfbDisplay;
}

int DFBWindowSystem::getIterationCounter()
{
     return m_iterationCounter;
}

