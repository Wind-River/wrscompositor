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
#include "Configuration.h"
#include "Shader.h"
#include "ShaderProgramGLES.h"
#include "TextureBinders/DFBEglImage.h"
#include "TextureBinders/DFBImage.h"
#include <pthread.h>
#include <signal.h>

DFBRenderer::DFBRenderer(ICommandExecutor& executor, Configuration& config)
: BaseRenderer(executor, config)
, m_num_outputs(0)
, m_dfb_mode_hw(true)
, m_dfb(NULL)
, m_screen(NULL)
, m_layer_windows(NULL)
, m_pWindowSystem(NULL)
, m_pDFBGraphicSystem(NULL)
, m_pGLESGraphicSystem(NULL)
, m_width(0)
, m_height(0)
, m_binder(NULL)
{
     memset( m_outputs, 0, sizeof(m_outputs) );

     if (getenv("IVI_DFBEGL"))
          m_dfb_mode_hw = false;
}

bool 
DFBRenderer::addLayer(int index, const DFBScreenDescription *sdesc, int layer_id)
{
     DFBDisplayLayerConfig config;
     DFBResult ret;
     char      screen_name[0xfff];
     int       width;
     int       height;

     if (m_pScene->isLayerInCurrentRenderOrder( layer_id )) {
          LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": layer(" << layer_id << ") is already in scene" );
          return false;
     }

     IDirectFBDisplayLayer *dfb_l;
     ret = m_dfb->GetDisplayLayer( m_dfb, layer_id, &dfb_l );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": IDirectFB::GetDisplayLayer(" << layer_id << ") failed! (ret=" << ret << ")" );
          return false;
     }

     ret = dfb_l->SetCooperativeLevel( dfb_l, DLSCL_EXCLUSIVE );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": IDirectFBDisplayLayer::SetCooperativeLevel(DLSCL_EXCLUSIVE) failed! (ret=" << ret << ")" );
          dfb_l->Release( dfb_l );
          return false;
     }

     if (!dfb_l->GetConfiguration( dfb_l, &config )) {
          config.flags = DLCONF_BUFFERMODE;
          config.buffermode = DLBM_TRIPLE;
          if (dfb_l->SetConfiguration( dfb_l, &config ))
               LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": failed to set layer configuration on layer " << dfb_l );
     }

     IDirectFBSurface *dfb_s;
     ret = dfb_l->GetSurface( dfb_l, &dfb_s );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": IDirectFBDisplayLayer::GetSurface() failed! (ret=" << ret << ")" );
          dfb_l->Release( dfb_l );
          return false;
     }

     ret = dfb_s->GetSize( dfb_s, &width, &height );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": IDirectFBSurface::GetSize() failed! (ret=" << ret << ")" );
          dfb_s->Release( dfb_s );
          dfb_l->Release( dfb_l );
          return false;
     }

     if (sdesc)
          sprintf( screen_name, "DirectFB Screen %d (%s %d:%d:%d:%d)", m_num_outputs, strlen(sdesc[index].name) > 0 ? sdesc[index].name : "Output", index, layer_id, m_outputs[m_num_outputs].encoder, m_outputs[m_num_outputs].mixer );
     else
          sprintf( screen_name, "DirectFB Screen %d (Layer %d:%d)", m_num_outputs, index, layer_id );

     LmScreen *lm_s = new LmScreen( m_num_outputs, screen_name );

     LmScreenList& sceneScreens = m_pScene->getScreenList();
     sceneScreens.push_back( lm_s );

     Layer *lm_l = m_pScene->createLayer( layer_id, getpid() );
     lm_l->setLayerType( m_dfb_mode_hw ? LayerType::Hardware : LayerType::Software_2D);
     lm_l->setVisibility( true );
     lm_l->setPosition( 0, 0 );
     lm_l->setDimension( width, height );

     if (m_dfb_mode_hw && !getenv("IVI_LAYER_SW"))
          lm_l->setLayerType( LayerType::Hardware );
     else
          lm_l->setLayerType( LayerType::Software_2D );

     const Rectangle r( 0, 0, width, height );
     lm_l->setSourceRegion( r );
     lm_l->setDestinationRegion( r );
     lm_l->OriginalSourceWidth = width;
     lm_l->OriginalSourceHeight = height;

     LayerList& screenLayers = lm_s->getCurrentRenderOrder();
     screenLayers.push_back( lm_l );

     LOG_INFO( "DFBRenderer", __FUNCTION__ << ": successfully added Screen " << m_num_outputs << " '" << screen_name << "'" );

     m_outputs[m_num_outputs].layer_id = layer_id;
     m_outputs[m_num_outputs].layer = dfb_l;
     m_outputs[m_num_outputs].surface = dfb_s;
     m_outputs[m_num_outputs].lm_screen = lm_s;
     m_outputs[m_num_outputs].lm_layer = lm_l;
     m_outputs[m_num_outputs].width = width;
     m_outputs[m_num_outputs].height = height;
     m_outputs[m_num_outputs].single = false;
     m_outputs[m_num_outputs].muted = false;
     m_outputs[m_num_outputs].num_surfaces = 0;
     m_outputs[m_num_outputs].layer_opacity = 1;

     m_num_outputs++;

     //m_outputs[m_num_outputs].mconf.layers = (1 << layer_id);
     //m_screen->SetMixerConfiguration( m_screen, m_outputs[m_num_outputs].mixer, &m_outputs[m_num_outputs].mconf );

     return true;
}

DFBEnumerationResult
DisplayLayerCallback( DFBDisplayLayerID           layer_id,
                      DFBDisplayLayerDescription  desc,
                      void                       *callbackdata )
{
     (void)desc;
     DFBRenderer *renderer = static_cast<DFBRenderer *>((DFBRenderer *)callbackdata);

     if (!renderer->addLayer( renderer->m_num_outputs, NULL, layer_id ))
          LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": failed to add layer with id " << layer_id << " to outputs" );

     return DFENUM_OK;
}

bool
DFBRenderer::addOutput(int index, const DFBScreenDescription *sdesc, const DFBScreenMixerDescription *mdescs, const DFBScreenEncoderDescription *edescs, const DFBScreenOutputDescription  *odescs)
{
     int layer_index;

     m_outputs[m_num_outputs].index = index;
     m_outputs[m_num_outputs].odesc = odescs[index];

     if (m_screen->GetOutputConfiguration( m_screen, index, &m_outputs[m_num_outputs].oconf )) {
          LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": failed to GetOutputConfiguration()" );
          m_outputs[m_num_outputs].encoder = 0;
     }

     if (m_outputs[m_num_outputs].oconf.flags & DSOCONF_ENCODER) {
          m_outputs[m_num_outputs].encoder = m_outputs[m_num_outputs].oconf.encoder;
          m_outputs[m_num_outputs].edesc   = edescs[m_outputs[m_num_outputs].encoder];

          if (m_screen->GetEncoderConfiguration( m_screen, m_outputs[m_num_outputs].encoder, &m_outputs[m_num_outputs].econf ))
               LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": failed to GetEncoderConfiguration()" );

          if (m_outputs[m_num_outputs].econf.flags & DSECONF_MIXER) {
               m_outputs[m_num_outputs].mixer = m_outputs[m_num_outputs].econf.mixer;
               m_outputs[m_num_outputs].mdesc = mdescs[m_outputs[m_num_outputs].mixer];

               if (m_screen->GetMixerConfiguration( m_screen, m_outputs[m_num_outputs].mixer, &m_outputs[m_num_outputs].mconf )) {
                    LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": failed to GetMixerConfiguration()" );
                    m_outputs[m_num_outputs].mconf.layers = (1 << DLID_PRIMARY);
               }

               int num_outputs = m_num_outputs;
               for (layer_index = 0; layer_index < 16; layer_index++) {
                    if (m_outputs[num_outputs].mconf.layers & (1 << layer_index)) {
                         if (!addLayer( index, sdesc, layer_index ))
                              LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": failed to add layer for output " << index );
                    }
               }
          }
     }
     else {
          LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": no encoder found, fallback to enumerated layers" );

          m_dfb->EnumDisplayLayers( m_dfb, DisplayLayerCallback, this );
     }

     return true;
}

bool DFBRenderer::init(int width, int height)
{
     DFBScreenDescription sdesc;
     DFBResult ret;
     int i;

     LOG_DEBUG( "DFBRenderer", __FUNCTION__ << "(width=" << width << ", height" << height << ")" );

     ret = DirectFBInit( 0, NULL );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": DirectFBInit() failed! (ret=" << ret << ")" );
          return false;
     }

     ret = DirectFBCreate( &m_dfb );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": DirectFBCreate() failed! (ret=" << ret << ")" );
          return false;
     }

     ret = m_dfb->GetScreen( m_dfb, DSCID_PRIMARY, &m_screen );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": IDirectFBScreen::GetScreen(DSCID_PRIMARY) failed! (ret=" << ret << ")" );
          return false;
     }

     ret = m_screen->GetDescription( m_screen, &sdesc );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": IDirectFBScreen::GetDescription() failed! (ret=" << ret << ")" );
          return false;
     }

     if (sdesc.outputs < 1) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": no screens available" );
          return false;
     }

     ret = m_dfb->GetDisplayLayer( m_dfb, DLID_PRIMARY, &m_layer_windows );
     if (ret) {
          LOG_ERROR( "DFBRenderer", __FUNCTION__ << ": GetDisplayLayer() failed! (ret=" << ret << ")" );
          return ret;
     }

     DFBScreenMixerDescription   mdescs[sdesc.mixers];
     DFBScreenEncoderDescription edescs[sdesc.encoders];
     DFBScreenOutputDescription  odescs[sdesc.outputs];

     m_screen->GetMixerDescriptions( m_screen, mdescs );
     m_screen->GetEncoderDescriptions( m_screen, edescs );
     m_screen->GetOutputDescriptions( m_screen, odescs );

     for (i = 0; i < sdesc.outputs && m_num_outputs < DFB_RENDERER_MAX_OUTPUTS - 1; i++) {
          if (!addOutput( i, &sdesc, mdescs, edescs, odescs ))
               LOG_WARNING( "DFBRenderer", __FUNCTION__ << ": failed to add screen for output " << i );
     }

     return m_num_outputs > 0;
}

bool DFBRenderer::start(int width, int height, const char *name, int maxIterationDurationInMS)
{
     (void)name;
     m_binder = NULL;
     m_width = width;
     m_height = height;

     LOG_INFO( "DFBRenderer", __FUNCTION__ << "(width=" << width << ", height" << height << ")" );

     if (!init( width, height ))
          return false;

     m_pWindowSystem = new DFBWindowSystem( width, height, m_pScene, m_pInputManager, m_dfb, m_outputs[0].layer );

     if (m_dfb_mode_hw) {
          m_pDFBGraphicSystem = new DFBGraphicSystem( this, m_pWindowSystem, m_dfb, width, height );

          if (!m_pWindowSystem->init( (BaseGraphicSystem<DFBDisplay, DFBWindow>*)m_pDFBGraphicSystem, m_dfb_mode_hw ))
               return false;

          m_pDFBGraphicSystem->setBaseWindowSystem( m_pWindowSystem );

          m_binder = new DFBImage( m_pWindowSystem->getNativeDisplayHandle(), m_layer_windows );
          if (m_binder) {
               m_pDFBGraphicSystem->setTextureBinder( m_binder );

               if (!m_pWindowSystem->start( maxIterationDurationInMS ))
                    return false;
          }
          else
               return false;
     }
     else {
          m_pGLESGraphicSystem = new GLESGraphicsystem( width, height, ShaderProgramGLES::createProgram );

          if (!m_pWindowSystem->init( (BaseGraphicSystem<DFBDisplay, DFBWindow>*)m_pGLESGraphicSystem, m_dfb_mode_hw ))
               return false;

          m_pGLESGraphicSystem->setBaseWindowSystem( m_pWindowSystem );

          m_binder = new DFBEglImage( m_pGLESGraphicSystem->getEGLDisplay(), m_layer_windows );
          if (m_binder) {
               m_pGLESGraphicSystem->setTextureBinder( m_binder );

               if (!m_pWindowSystem->start( maxIterationDurationInMS ))
                    return false;
          }
          else
               return false;
     }

     return true;
}

void DFBRenderer::stop()
{
     LOG_INFO( "DFBRenderer", __FUNCTION__ );

     if (m_pWindowSystem) {
          m_pWindowSystem->stop();
          delete m_pWindowSystem;
          m_pWindowSystem = NULL;
     }

     if (m_pDFBGraphicSystem) {
          delete m_pDFBGraphicSystem;
          m_pDFBGraphicSystem = NULL;
     }

     if (m_pGLESGraphicSystem) {
          delete m_pGLESGraphicSystem;
          m_pGLESGraphicSystem = NULL;
     }

     if (m_binder) {
          delete m_binder;
          m_binder = NULL;
     }

     for (int i = 0; i < m_num_outputs; i++) {
          m_outputs[i].layer->Release( m_outputs[i].layer );
          m_outputs[i].surface->Release( m_outputs[i].surface );
     }

     m_num_outputs = 0;

     if (m_layer_windows) {
          m_layer_windows->Release( m_layer_windows );
          m_layer_windows = NULL;
     }

     if (m_screen) {
          m_screen->Release( m_screen );
          m_screen = NULL;
     }

     if (m_dfb) {
          m_dfb->Release( m_dfb );
          m_dfb = NULL;
     }
}

void DFBRenderer::doScreenShot(std::string fileToSave, uint screen_id)
{
     m_pWindowSystem->doScreenShot( fileToSave, screen_id );
}

void DFBRenderer::doScreenShotOfLayer(std::string fileToSave, uint id)
{
     m_pWindowSystem->doScreenShotOfLayer( fileToSave, id );
}

void DFBRenderer::doScreenShotOfSurface(std::string fileToSave, uint id, uint layer_id)
{
     m_pWindowSystem->doScreenShotOfSurface( fileToSave, id, layer_id );
}

uint DFBRenderer::getNumberOfHardwareLayers(uint screenID)
{
     for (int i = 0; i < m_num_outputs; i++) {
          if (m_outputs[i].lm_screen->getID() == screenID)
               return 1;
     }

     return 0;
}

uint* DFBRenderer::getScreenResolution(uint screenID)
{
     for (int i = 0; i < m_num_outputs; i++) {
          if (m_outputs[i].lm_screen->getID() == screenID) {
               uint *resolution = new uint[2];
               resolution[0] = m_outputs[i].width;
               resolution[1] = m_outputs[i].height;
               return resolution;
          }
     }

     return NULL;
}

uint* DFBRenderer::getScreenIDs(uint* length)
{
     uint *screenIDS = NULL;

     if (m_num_outputs > 0) {
          screenIDS = new uint[m_num_outputs];
          for (int i = 0; i < m_num_outputs; i++)
               screenIDS[i] = m_outputs[i].lm_screen->getID();
     }

     *length = m_num_outputs;

     return screenIDS;
}

void DFBRenderer::signalWindowSystemRedraw()
{
     m_pWindowSystem->signalRedrawEvent();
}

void DFBRenderer::forceCompositionWindowSystem()
{
     m_pWindowSystem->m_forceComposition = true;
}

bool DFBRenderer::setOptimizationMode(OptimizationType id, OptimizationModeType mode)
{
     if (m_pDFBGraphicSystem)
          return false;

     return m_pGLESGraphicSystem->setOptimizationMode( id, mode );
}

bool DFBRenderer::getOptimizationMode(OptimizationType id, OptimizationModeType *mode)
{
     if (m_pDFBGraphicSystem)
          return false;

     return m_pGLESGraphicSystem->getOptimizationMode( id, mode );
}

int DFBRenderer::getIterationCounter()
{
     return m_pWindowSystem->getIterationCounter();
}

t_ilm_const_string DFBRenderer::pluginGetName() const
{
     return "DFBRenderer";
}

Shader* DFBRenderer::createShader(const string* vertexName, const string* fragmentName)
{
     if (m_pDFBGraphicSystem)
          return NULL;

     return Shader::createShader( *vertexName, *fragmentName );
}

DECLARE_LAYERMANAGEMENT_PLUGIN(DFBRenderer)
