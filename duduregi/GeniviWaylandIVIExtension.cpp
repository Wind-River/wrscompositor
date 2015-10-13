#include "GeniviWaylandIVIExtension.h"

using namespace GeniviWaylandIVIExtension;

Q_DECLARE_METATYPE(IVIScreen*)
Q_DECLARE_METATYPE(IVILayer*)
Q_DECLARE_METATYPE(IVISurface*)

IVIScene::IVIScene(int w, int h, QObject* parent) :
    IVIRectangle(0, w, h, parent)
{
    mMainScreen = new IVIScreen(0, w, h, this);
    // default layer
    mMainScreen->addLayer(1000);
    mMainScreen->addLayer(2000);
    mMainScreen->addLayer(3000);
    mMainScreen->addLayer(4000);
    mScreens << mMainScreen;
}

IVIScreen::IVIScreen(int id, int w, int h, QObject* parent) :
    IVIRectangle(id, w, h, parent)
{
}

void IVIScreen::addLayer(int id) {
    addLayer(id, mScene->width(), mScene->height());
}

void IVIScreen::addLayer(int id, int width, int height) {
    mLayers << new IVILayer(id, width, height);
}

IVILayer::IVILayer(int id, int w, int h, QObject* parent) :
    IVIRectangle(id, 0, 0, w, h, parent)
{
}

IVILayer::IVILayer(int id, int x, int y, int w, int h, QObject* parent) :
    IVIRectangle(id, x, y, w, h, parent)
{
}

IVISurface::IVISurface(int id, int w, int h, QObject* parent) :
    IVIRectangle(id, 0, 0, w, h, parent)
{
}

IVISurface::IVISurface(int id, int x, int y, int w, int h, QObject* parent) :
    IVIRectangle(id, x, y, w, h, parent)
{
}
