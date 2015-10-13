#include "GeniviWaylandIVIExtension.h"

using namespace GeniviWaylandIVIExtension;

Q_DECLARE_METATYPE(IVIScreen*)
Q_DECLARE_METATYPE(IVILayer*)
Q_DECLARE_METATYPE(IVISurface*)

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
    mMainScreen->addLayer(1000);
    mMainScreen->addLayer(2000);
    mMainScreen->addLayer(3000);
    mMainScreen->addLayer(4000);
}

IVIScreen::IVIScreen(int id, int w, int h, IVIScene* parent) :
    IVIRectangle(id, w, h, parent), mScene(parent)
{
}

void IVIScreen::addLayer(int id) {
    addLayer(id, mScene->width(), mScene->height());
}

void IVIScreen::addLayer(int id, int width, int height) {
    mLayers << new IVILayer(id, width, height);
}

IVILayer::IVILayer(int id, int w, int h, IVIScreen* parent) :
    IVIRectangle(id, 0, 0, w, h, parent), mScreen(parent)
{
}

IVILayer::IVILayer(int id, int x, int y, int w, int h, IVIScreen* parent) :
    IVIRectangle(id, x, y, w, h, parent), mScreen(parent)
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
