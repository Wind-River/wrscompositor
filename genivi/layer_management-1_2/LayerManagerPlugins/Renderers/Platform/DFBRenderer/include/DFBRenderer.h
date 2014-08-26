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

#ifndef _DFBRENDERER_H_
#define _DFBRENDERER_H_

#include "BaseRenderer.h"
#include "LayerList.h"
#include "WindowSystems/DFBWindowSystem.h"

typedef struct
{
     int                          index;
     int                          layer_id;
     IDirectFBDisplayLayer       *layer;
     IDirectFBSurface            *surface;
     LmScreen                    *lm_screen;
     Layer                       *lm_layer;
     int                          width;
     int                          height;
     int                          mixer;
     int                          encoder;
     DFBScreenMixerDescription    mdesc;
     DFBScreenEncoderDescription  edesc;
     DFBScreenOutputDescription   odesc;
     DFBScreenMixerConfig         mconf;
     DFBScreenEncoderConfig       econf;
     DFBScreenOutputConfig        oconf;
     bool                         single;
     bool                         muted;
     int                          num_surfaces;
     double                       layer_opacity;
} DFBRendererOutput;

#define DFB_RENDERER_MAX_OUTPUTS 0xFF

class DFBRenderer : public BaseRenderer {
public:
     DFBRenderer(ICommandExecutor& executor, Configuration& config);
     bool addLayer(int index, const DFBScreenDescription *sdesc, int layer_id);
     bool addOutput(int index, const DFBScreenDescription *sdesc, const DFBScreenMixerDescription *mdescs, const DFBScreenEncoderDescription *edescs, const DFBScreenOutputDescription  *odescs);
     bool init(int width, int height);
     bool start(int, int, const char*, int);
     void stop();
     void doScreenShot(std::string fileToSave, uint screen_id);
     void doScreenShotOfLayer(std::string fileToSave, uint id);
     void doScreenShotOfSurface(std::string fileToSave, uint id, uint layer_id);
     uint getNumberOfHardwareLayers(uint screenID);
     uint* getScreenResolution(uint screenID);
     uint* getScreenIDs(uint* length);
     void signalWindowSystemRedraw();
     void forceCompositionWindowSystem();
     Shader* createShader(const string* vertexName, const string* fragmentName);
     virtual bool setOptimizationMode(OptimizationType id, OptimizationModeType mode);
     virtual bool getOptimizationMode(OptimizationType id, OptimizationModeType *mode);
     virtual int getIterationCounter();
     virtual t_ilm_const_string pluginGetName() const;
public:
     int m_num_outputs;
     DFBRendererOutput m_outputs[DFB_RENDERER_MAX_OUTPUTS];
private:
     bool m_dfb_mode_hw;
     IDirectFB *m_dfb;
     IDirectFBScreen *m_screen;
     IDirectFBDisplayLayer *m_layer_windows;
     DFBWindowSystem *m_pWindowSystem;
     DFBGraphicSystem *m_pDFBGraphicSystem;
     GLESGraphicsystem *m_pGLESGraphicSystem;
     uint m_width;
     uint m_height;
     ITextureBinder *m_binder;

     friend DFBEnumerationResult DisplayLayerCallback(DFBDisplayLayerID layer_id, DFBDisplayLayerDescription desc, void *callbackdata);
};

#endif

