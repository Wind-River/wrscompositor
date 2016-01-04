#include "duduregiconfig.h"
#include "duduregicompositor.h"
#include "reardisplay.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>

RearDisplay::RearDisplay(QWindow *parent)
    : QQuickView(parent)
{
    setTitle(QLatin1String("Wind River Duduregi Rear Display Example"));
    setSource(QUrl("rearmain.qml"));
    setResizeMode(QQuickView::SizeRootObjectToView);

    QScreen *screen = QGuiApplication::primaryScreen();
    if(QGuiApplication::screens().count() > 1) {
        screen = QGuiApplication::screens().at(3); // third
        setScreen(screen);
    }
    setGeometry(screen->availableGeometry());
    QObject::connect(this, SIGNAL(windowSwapped(QVariant)), rootObject(), SLOT(windowSwapped(QVariant)));
    QObject::connect(this, SIGNAL(windowCloned(QVariant)), rootObject(), SLOT(windowCloned(QVariant)));
    QObject::connect(this, SIGNAL(windowCloneClosed(QVariant)), rootObject(), SLOT(windowCloneClosed(QVariant)));
    QObject::connect(rootObject(), SIGNAL(swappedWindowRestoreRequested(QVariant)), this, SLOT(slotSwappedWindowRestore(QVariant)));
    QObject::connect(rootObject(), SIGNAL(clonedWindowRestoreRequested(QVariant)), this, SLOT(slotClonedWindowRestore(QVariant)));
    QObject::connect(rootObject(), SIGNAL(clonedSurfaceItemDestroyed(QVariant)), this, SLOT(slotClonedSurfaceDestroy(QVariant)));
}

RearDisplay::~RearDisplay() {
}
void RearDisplay::addSwappedWindow(QQuickItem *windowFrame) {
    emit windowSwapped(QVariant::fromValue(windowFrame));
}
void RearDisplay::addClonedWindow(QWaylandSurfaceItem *item) {
    emit windowCloned(QVariant::fromValue(item));
}
void RearDisplay::closeClonedWindow(QWaylandQuickSurface *surface) {
    emit windowCloneClosed(QVariant::fromValue(surface));
}

void RearDisplay::slotSwappedWindowRestore(const QVariant &v) {
    QQuickItem *windowFrame = qobject_cast<QQuickItem*>(v.value<QObject*>());
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(windowFrame->property("child").value<QObject*>());
    qDebug() << windowFrame;
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    surface->setMainOutput(mMainOutput);
    windowFrame->setParent(mMainDisplay->rootObject());
    windowFrame->setParentItem(mMainDisplay->rootObject());
    /*
    QWaylandSurfaceLeaveEvent *le = new QWaylandSurfaceLeaveEvent(mMainOutput);
    QWaylandSurfaceEnterEvent *ee = new QWaylandSurfaceEnterEvent(mRearOutput);
    qApp->sendEvent(surface, le);
    qApp->sendEvent(surface, ee);
    qApp->flush();
    mRearDisplay->update();
    */
    qobject_cast<DuduregiCompositor*>(mMainDisplay)->restoreSwappedWindow(windowFrame);
}

void RearDisplay::slotClonedSurfaceDestroy(const QVariant &v) {
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(v.value<QObject*>());
    surfaceItem->deleteLater();
}
