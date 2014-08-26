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
#include "GraphicSystems/DFBGraphicSystem.h"
#include "WindowSystems/DFBWindowSystem.h"
#include "Bitmap.h"
#include <string.h>

extern "C" {
     #include <gfx/convert.h>
}

DFBGraphicSystem::DFBGraphicSystem(DFBRenderer *renderer, DFBWindowSystem *winsys, IDirectFB *dfb, int WindowWidth, int WindowHeight)
: m_renderer(renderer)
, m_winsys(winsys)
, m_renderer_output(0)
, m_windowWidth(WindowWidth)
, m_windowHeight(WindowHeight)
, m_display(0)
, m_window(0)
, m_dfb(dfb)
, m_currentLayer(NULL)
, m_single_surface(NULL)
, m_single_x(0)
, m_single_y(0)
{
}

DFBGraphicSystem::~DFBGraphicSystem()
{
}

void DFBGraphicSystem::activateGraphicContext()
{
}

void DFBGraphicSystem::releaseGraphicContext()
{
}

bool DFBGraphicSystem::init(DFBDisplay display, DFBWindow window)
{
     (void)window;

     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ << "(display=" << display << ", window=" << window << ")" );

     m_display = display;

     return true;
}

void DFBGraphicSystem::clearBackground()
{
     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ );

     if (m_renderer_output < 0) {
          LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": renderer output not set!\n" );
          return;
     }

     if (!m_renderer->m_outputs[m_renderer_output].single)
          m_renderer->m_outputs[m_renderer_output].surface->Clear( m_renderer->m_outputs[m_renderer_output].surface, 0, 0, 0, 0 );
}

void DFBGraphicSystem::swapBuffers()
{
     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ );

     if (m_renderer_output < 0) {
          LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": renderer output not set!\n" );
          return;               
     }

     if (!m_renderer->m_outputs[m_renderer_output].single)
          m_renderer->m_outputs[m_renderer_output].surface->Flip( m_renderer->m_outputs[m_renderer_output].surface, NULL, DSFLIP_NONE );
}

void DFBGraphicSystem::beginLayer(Layer *currentLayer)
{
     int i;

     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ << "(" << currentLayer << ")" );

     m_currentLayer = currentLayer;
     m_renderer_output = -1;

     for (i = 0; i < m_renderer->m_num_outputs; i++) {
          if (m_renderer->m_outputs[i].layer_id == (int)currentLayer->getID()) {
               m_renderer_output = i;
               break;
          }
     }
}

bool DFBGraphicSystem::needsRedraw(Layer *layer)
{
     if (layer->renderPropertyChanged)
          return true;

     if (layer->visibility && layer->opacity > 0.0) {
          SurfaceList surfaces = layer->getAllSurfaces();
          for (SurfaceListConstIterator currentS = surfaces.begin(); currentS != surfaces.end(); currentS++) {
               if ((*currentS)->renderPropertyChanged)
                    return true;

               if ((*currentS)->hasNativeContent() && (*currentS)->damaged && (*currentS)->visibility && (*currentS)->opacity > 0.0f)
                    return true;
          }
     }

     return false;
}

bool DFBGraphicSystem::needsRedraw(LayerList layers)
{
     for (LayerListConstIterator layer = layers.begin(); layer != layers.end(); layer++) {
          if ((*layer)->getLayerType() == Hardware && layers.size() > 1)
               LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": called with layers not in the same composition" );

          if (needsRedraw( *layer ))
               return true;
     }

     return false;
}

void DFBGraphicSystem::renderSWLayer(Layer *layer, bool clear)
{
     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ << "(layer=" << layer << ", clear=" << clear << ")" );

     beginLayer( layer );

     if (layer->getLayerType() != Software_2D && layer->getLayerType() != Software_2_5D)
          LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": called with a non-SW layer" );

     if (layer->visibility && layer->opacity > 0.0f) {
          if (m_currentLayer->getChromaKeyEnabled())
               LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": cannot handle chroma key" );

          if (clear)
               clearBackground();

          SurfaceList surfaces = m_currentLayer->getAllSurfaces();
          for (std::list<Surface*>::const_iterator currentS = surfaces.begin(); currentS != surfaces.end(); ++currentS) {
               if ((*currentS)->hasNativeContent() && (*currentS)->visibility && (*currentS)->opacity > 0.0f)
                    renderSurface( *currentS );
          }
     }

     endLayer();
}

void DFBGraphicSystem::renderSWLayers(LayerList layers, bool clear)
{
     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ << "(clear=" << clear << ")" );

     for (LayerListConstIterator layer = layers.begin(); layer != layers.end(); layer++)
          renderSWLayer( *layer, clear );
}

void DFBGraphicSystem::endLayer()
{
     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ );

     m_currentLayer = NULL;
     m_renderer_output = -1;
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

void DFBGraphicSystem::renderSurface(Surface *currentSurface)
{
     int num_surfaces = countVisibleSurfaces( m_currentLayer );
     FloatRectangle target_src;
     FloatRectangle target_dst;
     DFBRectangle src_rect;
     DFBRectangle dst_rect;
     DFBSurfaceBlittingFlags flags = DSBLIT_NOFX;
     DFBSurfaceCapabilities caps;
     DFBSurfacePixelFormat format;
     double opacity = currentSurface->getOpacity();
     bool blitting_fallback = false;

     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ << "(surface=" << currentSurface << ")" );

     if (m_renderer_output < 0) {
          LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": renderer output not set!" );
          return;
     }

     if (!currentSurface->platform) {
          LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": Platform surface not available" );
          return;
     }

     target_src = currentSurface->getTargetSourceRegion();
     src_rect.x = target_src.x;
     src_rect.y = target_src.y;
     src_rect.w = target_src.width;
     src_rect.h = target_src.height;

     target_dst = currentSurface->getTargetDestinationRegion();
     dst_rect.x = target_dst.x;
     dst_rect.y = target_dst.y;
     dst_rect.w = target_dst.width;
     dst_rect.h = target_dst.height;

     DFBPlatformSurface* platformSurface = (DFBPlatformSurface *)currentSurface->platform;

     LOG_DEBUG( "DFBGraphicsSystem", __FUNCTION__ << ": render DirectFB surface " << platformSurface->dfb_surface <<
                " (src " << src_rect.x << "," << src_rect.y << "," << src_rect.w << "," << src_rect.h <<
                ") (dst " << dst_rect.x << "," << dst_rect.y << "," << dst_rect.w << "," << dst_rect.h <<
                ") with id " << platformSurface->dfb_surface_id << " (visible surfaces on layer: " << num_surfaces << ")" );

     if (m_currentLayer->getLayerType() != Hardware)
          opacity *= m_currentLayer->getOpacity();

     while (num_surfaces == 1) {
          if (src_rect.w != dst_rect.w || src_rect.h != dst_rect.h) {
               blitting_fallback = true;
               break;
          }

          if (!m_renderer->m_outputs[m_renderer_output].single || m_single_surface != platformSurface->dfb_surface) {
               if (m_renderer->m_outputs[m_renderer_output].layer->SetSurface( m_renderer->m_outputs[m_renderer_output].layer, platformSurface->dfb_surface )) {
                    LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": failed to set surface " << platformSurface->dfb_surface << " on layer " << m_renderer->m_outputs[m_renderer_output].layer );
               }
               else {
                    LOG_INFO( "DFBGraphicSystem", __FUNCTION__ << ": switching single mode ON for layer " << m_renderer->m_outputs[m_renderer_output].layer );

                    m_single_surface = platformSurface->dfb_surface;
                    m_renderer->m_outputs[m_renderer_output].single = true;
                    //m_winsys->m_ignore_surface_updates = true;
               }
          }

          if (m_single_x != dst_rect.x || m_single_y != dst_rect.y) {
               if (m_renderer->m_outputs[m_renderer_output].layer->SetScreenPosition( m_renderer->m_outputs[m_renderer_output].layer, dst_rect.x, dst_rect.y )) {
                    LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": failed to set screen location of surface " << platformSurface->dfb_surface << " on layer " << m_renderer->m_outputs[m_renderer_output].layer );
               }
               else {
                    m_single_x = dst_rect.x;
                    m_single_y = dst_rect.y;

                    LOG_INFO( "DFBGraphicSystem", __FUNCTION__ << ": set screen location " << m_single_x << "," << m_single_y << " for layer " << m_renderer->m_outputs[m_renderer_output].layer );
               }
          }

          break;
     }

     if (num_surfaces > 1 || blitting_fallback) {
          if (m_renderer->m_outputs[m_renderer_output].single || blitting_fallback) {
               if (m_single_surface != m_renderer->m_outputs[m_renderer_output].surface) {
                    LOG_INFO( "DFBGraphicSystem", __FUNCTION__ << ": switching single mode OFF for layer " << m_renderer->m_outputs[m_renderer_output].layer );

                    if (m_renderer->m_outputs[m_renderer_output].layer->SetSurface( m_renderer->m_outputs[m_renderer_output].layer, m_renderer->m_outputs[m_renderer_output].surface )) {
                         LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": failed to set surface " << m_renderer->m_outputs[m_renderer_output].surface << " on layer " << m_renderer->m_outputs[m_renderer_output].layer );
                    }
                    else {
                         m_single_surface = m_renderer->m_outputs[m_renderer_output].surface;
                         m_renderer->m_outputs[m_renderer_output].single = false;
                         m_winsys->m_ignore_surface_updates = false;
                    }
               }
          }

          if (opacity > 0 && opacity < 1) {
               platformSurface->dfb_surface->SetPorterDuff( platformSurface->dfb_surface, DSPD_SRC_OVER );

               if (platformSurface->dfb_surface->GetPixelFormat( platformSurface->dfb_surface, &format )) {
                    LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": failed to retrieve pixelformat from surface " << platformSurface->dfb_surface << " with id " << platformSurface->dfb_surface_id );
               }
               else if (DFB_PIXELFORMAT_HAS_ALPHA(format))
                    flags = (DFBSurfaceBlittingFlags)(flags | DSBLIT_BLEND_ALPHACHANNEL);

               if (platformSurface->dfb_surface->GetCapabilities( platformSurface->dfb_surface, &caps )) {
                    LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": failed to retrieve caps from surface " << platformSurface->dfb_surface << " with id " << platformSurface->dfb_surface_id );
               }
               else {
                    if (caps & DSCAPS_PREMULTIPLIED)
                         flags = (DFBSurfaceBlittingFlags)(flags | DSBLIT_SRC_PREMULTCOLOR);
                    else
                         flags = (DFBSurfaceBlittingFlags)(flags | DSBLIT_BLEND_COLORALPHA | DSBLIT_SRC_PREMULTIPLY);
               }

               m_renderer->m_outputs[m_renderer_output].surface->SetColor( m_renderer->m_outputs[m_renderer_output].surface, 0, 0, 0, opacity * 0xFF );
          }

          m_renderer->m_outputs[m_renderer_output].surface->SetBlittingFlags( m_renderer->m_outputs[m_renderer_output].surface, flags );
          m_renderer->m_outputs[m_renderer_output].surface->StretchBlit( m_renderer->m_outputs[m_renderer_output].surface, platformSurface->dfb_surface, &src_rect, &dst_rect );
     }
}

void DFBGraphicSystem::saveScreenShotOfFramebuffer(std::string fileToSave)
{
     DFBResult              ret;
     IDirectFBSurface      *primary;
     DFBSurfacePixelFormat  format;
     void                  *primary_data;
     int                    primary_pitch;
     int                    primary_width;
     int                    primary_height;
     u8                    *rgb_data;

     LOG_DEBUG( "DFBGraphicSystem", __FUNCTION__ << "(" << fileToSave << ") (output=" << m_renderer_output << ")" );

     if (m_renderer_output < 0) {
          LOG_WARNING( "DFBGraphicSystem", __FUNCTION__ << ": renderer output not set!" );
          return;
     }

     ret = m_renderer->m_outputs[m_renderer_output].layer->GetSurface( m_renderer->m_outputs[m_renderer_output].layer, &primary );
     if (ret) { 
          LOG_ERROR( "DFBGraphicSystem", __FUNCTION__ << ": IDirectFBDisplayLayer::GetSurface() failed! (ret=" << ret << ")" );
          return;
     }

     ret = primary->GetSize( primary, &primary_width, &primary_height );
     if (ret) { 
          LOG_ERROR( "DFBGraphicSystem", __FUNCTION__ << ": IDirectFBSurface::GetSize() failed! (ret=" << ret << ")" );
          primary->Release( primary );
          return;
     }

     ret = primary->GetPixelFormat( primary, &format );
     if (ret) {
          LOG_ERROR( "DFBGraphicSystem", __FUNCTION__ << ": failed to retrieve pixelformat from primary " << primary << "(ret=" << ret << ")" );
          return;
     }

     ret = primary->Lock( primary, DSLF_READ, &primary_data, &primary_pitch );
     if (ret) {
          LOG_ERROR( "DFBGraphicSystem", __FUNCTION__ << ": failed to lock primary " << primary << "(ret=" << ret << ")" );
          primary->Release( primary );
          return;
     }

     rgb_data = (u8 *)malloc( primary_width * primary_height * 3 * sizeof(u8) );
     if (!rgb_data) {
          LOG_ERROR( "DFBGraphicSystem", __FUNCTION__ << ": failed to allocate screenshot buffer for primary " << primary );
          primary->Unlock( primary );
          primary->Release( primary );
          return;
     }

     dfb_convert_to_rgb24( format, primary_data, primary_pitch, NULL, 0, NULL, 0, primary_height, rgb_data, primary_width * 3, primary_width, primary_height );

     primary->Unlock( primary );
     primary->Release( primary );

     writeBitmap( fileToSave, (char *)rgb_data, primary_width, primary_height );

     free( rgb_data );
}
