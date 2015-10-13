#include "GeniviWaylandIVIExtension.h"

using namespace GeniviWaylandIVIExtension;


IVIScene::IVIScene(QObject* parent) :
    QObject(parent)
{
}

IVIScreen::IVIScreen(QObject* parent) :
    QObject(parent)
{
}

IVILayer::IVILayer(QObject* parent) :
    QObject(parent)
{
}

IVISurface::IVISurface(QObject* parent) :
    QObject(parent)
{
}
