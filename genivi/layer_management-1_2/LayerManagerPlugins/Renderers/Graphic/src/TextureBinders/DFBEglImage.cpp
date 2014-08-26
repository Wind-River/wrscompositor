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

#include "TextureBinders/DFBEglImage.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <GLES2/gl2ext.h>
#include "PlatformSurfaces/EglDFBPlatformSurface.h"

#include <EGL/egldfbext.h>

DFBEglImage::DFBEglImage(EGLDisplay eglDisplay, IDirectFBDisplayLayer* layer)
: m_eglDisplay(eglDisplay)
, m_layer(layer)
{
     m_pfGLEglImageTargetTexture2DOES = (PFNGLEGLIMAGETARGETTEXTURE2DOESPROC)eglGetProcAddress("glEGLImageTargetTexture2DOES");
     m_pfEglCreateImageKHR = (PFNEGLCREATEIMAGEKHRPROC)eglGetProcAddress("eglCreateImageKHR");
     m_pfEglDestroyImageKHR = (PFNEGLDESTROYIMAGEKHRPROC)eglGetProcAddress("eglDestroyImageKHR");

     if (!m_pfEglCreateImageKHR || !m_pfEglDestroyImageKHR || !m_pfGLEglImageTargetTexture2DOES)
          LOG_ERROR( "DFBEglImage", __FUNCTION__ << ": query EGL extensions failed" );
}

bool DFBEglImage::bindSurfaceTexture(Surface* surface)
{
     EglDFBPlatformSurface* nativeSurface = (EglDFBPlatformSurface*)surface->platform;

     if (nativeSurface && nativeSurface->isReadyForRendering()) {
          glBindTexture(GL_TEXTURE_2D, nativeSurface->texture);
          return true;
     }

     return false;
}

bool DFBEglImage::unbindSurfaceTexture(Surface* surface)
{
     (void)surface; // TODO: remove, only prevents warning

     // TODO
     return true;
}

void DFBEglImage::createClientBuffer(Surface* surface)
{
     EglDFBPlatformSurface* nativeSurface = (EglDFBPlatformSurface*)surface->platform;

     if (NULL!=nativeSurface) {
          EGLImageKHR eglImage = 0;

          if (nativeSurface->eglImage) {
               m_pfEglDestroyImageKHR(m_eglDisplay, nativeSurface->eglImage);
               glDeleteTextures(1,&nativeSurface->texture);
               nativeSurface->eglImage = 0;
               nativeSurface->texture = 0;
          }

          eglImage = m_pfEglCreateImageKHR(m_eglDisplay,
                                           EGL_NO_CONTEXT,
                                           EGL_IMAGE_IDIRECTFBSURFACE_DIRECTFB,
                                           (EGLClientBuffer)nativeSurface->dfb_surface,
                                           NULL);
          if (!eglImage) {
               LOG_WARNING( "DFBEglImage", __FUNCTION__ << ": could not allocate EGL Image for surface " << surface << " with id " << surface->getNativeContent());
          }
          else {
               nativeSurface->eglImage = eglImage;
               glGenTextures(1,&nativeSurface->texture);
               glBindTexture(GL_TEXTURE_2D, nativeSurface->texture);
               glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
               glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
               glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
               glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
               m_pfGLEglImageTargetTexture2DOES(GL_TEXTURE_2D, nativeSurface->eglImage);
          }
     }
}

PlatformSurface* DFBEglImage::createPlatformSurface(Surface* surface)
{
     return new EglDFBPlatformSurface(surface, m_layer);
}

void DFBEglImage::destroyClientBuffer(Surface* surface)
{
     EglDFBPlatformSurface* nativeSurface = (EglDFBPlatformSurface*)surface->platform;
     if (nativeSurface && nativeSurface->eglImage) {
          m_pfEglDestroyImageKHR(m_eglDisplay, nativeSurface->eglImage);
          glDeleteTextures(1,&nativeSurface->texture);
          nativeSurface->eglImage = 0;
          nativeSurface->texture = 0;
     }
}
