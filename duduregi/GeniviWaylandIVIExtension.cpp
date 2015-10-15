#include "GeniviWaylandIVIExtension.h"

using namespace GeniviWaylandIVIExtension;

Q_DECLARE_METATYPE(IVIScreen*)
Q_DECLARE_METATYPE(IVILayer*)
Q_DECLARE_METATYPE(IVISurface*)

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

IVISurface* IVILayer::addSurface(int x, int y, int width, int height, QObject *qmlWindowFrame) {
    IVISurface* surface = new IVISurface(-1, width, height, this);
    surface->setX(x);
    surface->setY(y);
    qDebug() << "setsetQmlWindowFrame" << qmlWindowFrame;
    surface->setQmlWindowFrame(qmlWindowFrame);
    mSurfaces << surface;
    return surface;
}

IVISurface::IVISurface(QObject *parent) :
    IVIRectangle(-1, 0, 0, 0, 0, parent), mLayer(NULL)
{
}

IVISurface::IVISurface(int id, int w, int h, IVILayer* parent) :
    IVIRectangle(id, 0, 0, w, h, parent), mLayer(parent)
{
}

IVISurface::IVISurface(int id, int x, int y, int w, int h, IVILayer* parent) :
    IVIRectangle(id, x, y, w, h, parent), mLayer(parent)
{
}
