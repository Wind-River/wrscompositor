/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
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
    QUrl programUrl = QUrl("qrc:///rearmain.qml");
    if(qApp->arguments().contains("--debug"))
        programUrl = QUrl("hmi/" DUDUREGI_HMI_PROFILE "/rearmain.qml");
    setSource(programUrl);
    setResizeMode(QQuickView::SizeRootObjectToView);

    QScreen *screen = QGuiApplication::primaryScreen();
    if(QGuiApplication::screens().count() > 1) {
        screen = QGuiApplication::screens().at(2); // third
        setScreen(screen);
    }
    setGeometry(screen->availableGeometry());
    QObject::connect(this, SIGNAL(windowSwapped(QVariant)), rootObject(), SLOT(windowSwapped(QVariant)));
    QObject::connect(this, SIGNAL(windowCloned(QVariant)), rootObject(), SLOT(windowCloned(QVariant)));
    QObject::connect(this, SIGNAL(windowCloneClosed(QVariant)), rootObject(), SLOT(windowCloneClosed(QVariant)));
    QObject::connect(rootObject(), SIGNAL(swappedWindowRestoreRequested(QVariant)), this, SLOT(slotSwappedWindowRestore(QVariant)));
    //QObject::connect(rootObject(), SIGNAL(clonedWindowRestoreRequested(QVariant)), this, SLOT(slotClonedWindowRestore(QVariant)));
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
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(v.value<QObject*>());
    //QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    //surface->setMainOutput(mMainOutput);
    /*
    QWaylandSurfaceLeaveEvent *le = new QWaylandSurfaceLeaveEvent(mMainOutput);
    QWaylandSurfaceEnterEvent *ee = new QWaylandSurfaceEnterEvent(mRearOutput);
    qApp->sendEvent(surface, le);
    qApp->sendEvent(surface, ee);
    qApp->flush();
    mRearDisplay->update();
    */
    qobject_cast<DuduregiCompositor*>(mMainDisplay)->restoreSwappedWindow(surfaceItem);
    surfaceItem->deleteLater();
}

void RearDisplay::slotClonedSurfaceDestroy(const QVariant &v) {
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(v.value<QObject*>());
    surfaceItem->deleteLater();
}
