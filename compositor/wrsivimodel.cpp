/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <QtCompositor/qwaylandquicksurface.h>
#include <QtCompositor/qwaylandclient.h>

#include "wrsivimodel.h"
#include "wrslogging.h"
#include "util.h"
#include "config.h"

using namespace WrsIVIModel;

Q_DECLARE_METATYPE(IVIScreen*)
Q_DECLARE_METATYPE(IVILayer*)
Q_DECLARE_METATYPE(IVISurface*)

////////////////////////////////////////////////////////////////////////////////
/// IVIScene
////////////////////////////////////////////////////////////////////////////////

IVIScene::IVIScene(QObject* parent) :
    IVIRectangle(-1, 0, 0, parent), mMainScreen(0), mCompositor(0)
{
}

IVIScene::IVIScene(QWaylandCompositor* compositor, int w, int h, QObject* parent) :
    IVIRectangle(0, w, h, parent), mMainScreen(0), mCompositor(compositor)
{
    QList<QWaylandOutput *> outputs = mCompositor->outputs();
    Q_FOREACH(QWaylandOutput *output, mCompositor->outputs()) {
        IVIScreen *screen = new IVIScreen(0, w, h, this);
        screen->setWaylandOutput(output);
        mScreens << screen;
        if(output == compositor->primaryOutput())
            mMainScreen = screen;
    }
    Q_ASSERT(mMainScreen);
    if (!strcmp(WRSCOMPOSITOR_HMI_PROFILE, "classic")) {
        // default layer
        mMainScreen->addLayer(1000); // for application
        mMainScreen->layer(mMainScreen->layerCount()-1)->setVisibility(1);
        mMainScreen->layer(mMainScreen->layerCount()-1)->setOpacity(1);
        mMainScreen->addLayer(2000); // for main menu
        mMainScreen->layer(mMainScreen->layerCount()-1)->setVisibility(1);
        mMainScreen->layer(mMainScreen->layerCount()-1)->setOpacity(1);
        mMainScreen->addLayer(3000); // for status bar
        mMainScreen->layer(mMainScreen->layerCount()-1)->setVisibility(1);
        mMainScreen->layer(mMainScreen->layerCount()-1)->setOpacity(1);
    }
}


IVISurface* IVIScene::findSurfaceByResource(struct ::wl_resource *rsc) {
    WrsIVIModel::IVISurface *surface = NULL;
    for (int i = 0; i < this->screenCount(); i++) {
        WrsIVIModel::IVIScreen *screen = this->screen(i);
        for (int j = 0; j < screen->layerCount(); j++) {
            WrsIVIModel::IVILayer *layer = screen->layer(j);
            for (int k = 0; k < layer->surfaceCount(); k++) {
                WrsIVIModel::IVISurface *_surface = layer->surface(k);
                if (_surface->getResourceForClient(rsc->client) == rsc) {
                    surface = _surface;
                    break;
                }
            }
        }
    }
    return surface;
}


QString IVIScene::getSurfaceRole(QWaylandSurface *qWlSurface) {
    IVISurface* iviSurface = findIVISurfaceByQWaylandSurface(qWlSurface);
    if (iviSurface->iviId() == WRS_IVI_ID_SURFACE_CAMERA) {
        return "Camera";
    } else if (iviSurface->iviId() == WRS_IVI_ID_SURFACE_DIALOG) {
        return "Dialog";
    } else if (iviSurface->iviId() == WRS_IVI_ID_SURFACE_NAVIGATION) {
        return "Navigation";
    } else if (iviSurface->iviId() == WRS_IVI_ID_SURFACE_PHONE) {
        return "Phone";
    } else if (iviSurface->iviId() == WRS_IVI_ID_SURFACE_PROJECTION) {
        return "Projection";
    } else {
        Util u;
        return u.getCmdForPid(qWlSurface->client()->processId());
    }
}


IVISurface* IVIScene::findIVISurfaceByQWaylandSurface(QWaylandSurface *qWlSurface) {
    DEBUG() << "search:" << qWlSurface;
    for (int i = 0; i < this->mIviSurfaces.count(); i++) {
        if (((IVISurface* ) (this->mIviSurfaces[i]))->qWaylandSurface() == qWlSurface) {
            DEBUG() << "found:" << ((IVISurface* ) (this->mIviSurfaces[i]));
            return ((IVISurface* ) (this->mIviSurfaces[i]));
        }
    }
    return NULL;
}


void IVIScene::addIVIScreen(IVIScreen *screen) {
    this->mScreens.append(screen);
}


void IVIScene::addIVILayer(IVILayer *layer) {
    //TODO: Add logic to associate the layers to specific screens
    //this->mainScreen()->layers().append(layer);
    this->mIviLayesr.append(layer);
}


void IVIScene::addIVISurface(IVISurface *surface) {
    DEBUG() << "ivi-surface:" << surface;
    this->mIviSurfaces.append(surface);
    if (!strcmp(WRSCOMPOSITOR_HMI_PROFILE, "classic")) {
        // TODO: Add logic to associate the surface to a specific layer (even a default one)
        // This is wrong :)
        this->mainScreen()->layer(0)->addSurface(surface);
    }
}


void IVIScene::removeIVISurface(IVISurface *surface) {
    this->mIviSurfaces.removeAll(surface);
    for (int i = 0; i < this->screenCount(); i++) {
        for (int j = 0; j < this->screen(i)->layerCount(); j++) {
            this->mScreens[i]->layer(j)->removeSurface(surface);
        }
    }
}


IVISurface* IVIScene::createSurface(int x, int y, int width, int height, QObject *qmlWindowFrame) {
    IVISurface* surface = new IVISurface(-1 - this->mIviSurfaces.count(), width, height, this->mainScreen()->layer(0));
    DEBUG() << "qmlWindowFrame" << qmlWindowFrame;
    surface->setX(x);
    surface->setY(y);
    surface->setQmlWindowFrame(qmlWindowFrame);
    return surface;
}


////////////////////////////////////////////////////////////////////////////////
/// IVIScreen
////////////////////////////////////////////////////////////////////////////////

IVIScreen::IVIScreen(QObject* parent) : IVIRectangle(-1, 0, 0, parent), mScene(0)
{
}

IVIScreen::IVIScreen(int id, int w, int h, IVIScene* parent) : IVIRectangle(id, w, h, parent), mScene(parent)
{
}

void IVIScreen::addLayer(int id) {
    addLayer(id, mScene->width(), mScene->height());
}

void IVIScreen::addLayer(int id, int width, int height) {
    mLayers << new IVILayer(id, width, height, this);
}

IVILayer* IVIScreen::layerById(int id) {
    Q_FOREACH(IVILayer* layer, mLayers) {
        if(layer->id() == id) {
            return layer;
        }
    }
    return NULL;
}

IVILayer::IVILayer(QObject* parent) :
    IVIRectangle(-1, 0, 0, 0, 0, parent), mScreen(0), mOpacity(0), mOrientation(0), mVisibility(0)
{
}

IVILayer::IVILayer(int id, int w, int h, IVIScreen* parent) :
    IVIRectangle(id, 0, 0, w, h, parent), mScreen(parent), mOpacity(0), mOrientation(0), mVisibility(0)
{
}

IVILayer::IVILayer(int id, int x, int y, int w, int h, IVIScreen* parent) :
    IVIRectangle(id, x, y, w, h, parent), mScreen(parent), mOpacity(0), mOrientation(0), mVisibility(0)
{
}


IVISurface* IVILayer::addSurface(IVISurface* surface) {
    DEBUG() << "ivi-surface <<<" << surface;
    mSurfaces << surface;
    return surface;
}


void IVILayer::removeSurface(IVISurface* surface) {
    mSurfaces.removeAll(surface);
}

IVISurface::IVISurface(QObject *parent) :
    IVIRectangle(-1, 0, 0, 0, 0, parent), mLayer(NULL), mIviId(WRS_IVI_ID_SURFACE_DEFAULT), mQWaylandSurface(NULL), mQmlWindowFrame(NULL)
{
}

IVISurface::IVISurface(int id, int w, int h, IVILayer* parent) :
    IVIRectangle(id, 0, 0, w, h, parent), mLayer(parent), mIviId(WRS_IVI_ID_SURFACE_DEFAULT), mQWaylandSurface(NULL), mQmlWindowFrame(NULL)
{
}

IVISurface::IVISurface(int id, int x, int y, int w, int h, IVILayer* parent) :
    IVIRectangle(id, x, y, w, h, parent), mLayer(parent), mIviId(WRS_IVI_ID_SURFACE_DEFAULT), mQWaylandSurface(NULL), mQmlWindowFrame(NULL)
{
}
