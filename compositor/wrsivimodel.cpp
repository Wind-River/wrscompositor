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
#include "wrsiviplatformconstants.h"
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

IVILayer* IVIScreen::addLayer(int id) {
    return addLayer(id, mScene->width(), mScene->height());
}

IVILayer* IVIScreen::addLayer(int id, int width, int height) {
    IVILayer* layer = new IVILayer(id, width, height, this);
    mLayers << layer;
    return layer;
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

IVISurface* IVILayer::addSurface(int x, int y, int width, int height,
                                QObject *qmlWindowFrame,  QWaylandSurface *qWaylandSurface) {
    IVISurface* surface = new IVISurface(-1, width, height, this, qWaylandSurface);
    surface->setX(x);
    surface->setY(y);
    surface->setQmlWindowFrame(qmlWindowFrame);
    mSurfaces << surface;
    return surface;
}

void IVILayer::removeSurface(IVISurface* surface) {
    mSurfaces.removeAll(surface);
}

IVISurface::IVISurface(QObject *parent) :
    IVIRectangle(WRS_IVI_ID_SURFACE_DEFAULT, 0, 0, 0, 0, parent), mLayer(NULL), mQWaylandSurface(NULL)
{
}

IVISurface::IVISurface(int id, int w, int h, IVILayer* parent,  QWaylandSurface *qWaylandSurface) :
    IVIRectangle(id, 0, 0, w, h, parent), mLayer(parent), mQWaylandSurface(qWaylandSurface)
{
}

IVISurface::IVISurface(int id, int x, int y, int w, int h, IVILayer* parent, QWaylandSurface *qWaylandSurface) :
    IVIRectangle(id, x, y, w, h, parent), mLayer(parent), mQWaylandSurface(qWaylandSurface)
{
}
