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

#ifndef WRS_COMPOSITOR
#define WRS_COMPOSITOR
#include "config.h"
#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
#include <QtCompositor/qwaylandsurfaceitem.h>
#include <QtCompositor/qwaylandoutput.h>
#include <QtCompositor/qwaylandclient.h>
#include <QtCompositor/qwaylandquickcompositor.h>
#include <QtCompositor/qwaylandquicksurface.h>
#include <QtCompositor/qwaylandquickoutput.h>
#include "qwayland-server-ivi-controller.h"
#include "qwayland-server-ivi-application.h"

//Compositor requires access to private classes on QtCompositor framework
#include <QtCompositor/private/qwlcompositor_p.h>
#include <QtCompositor/private/qwloutput_p.h>
#include <QtCompositor/private/qwlsubsurface_p.h>
#include <QtCompositor/private/qwlinputdevice_p.h>
#include <QtCompositor/private/qwayland-server-wayland.h>

#include "wrsivimodel.h"
#endif

#include <QSurfaceFormat>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QSettings>

#include <QTimer>
#if WRSCOMPOSITOR_REARDISPLAY
#include "reardisplay.h"
#endif

class WrsIviController;
class WrsIviApplication;
class WrsIviSurface;

class WrsCompositor : public QQuickView
#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
                      , public QWaylandQuickCompositor
#endif
{
    Q_OBJECT
#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
    Q_PROPERTY(QWaylandQuickSurface* fullscreenSurface READ fullscreenSurface WRITE setFullscreenSurface NOTIFY fullscreenSurfaceChanged)
#endif

public:
    WrsCompositor(const QString &display=QString::null, const QString &program=QString::null);
    ~WrsCompositor();

    QWaylandQuickOutput * getMainOutput() { return mMainOutput;}
    QWaylandQuickSurface * getFullscreenSurface() { return m_fullscreenSurface;}
    WrsIVIModel::IVIScene * getIviScene() { return mIviScene;}
    WrsIVIModel::IVISurface* findSurfaceByResource(struct ::wl_resource *rsc);
    Q_INVOKABLE WrsIVIModel::IVISurface* findIVISurfaceByQWaylandSurface(QWaylandSurface *qWlSurface);
    Q_INVOKABLE QString getSurfaceRole(QWaylandSurface *qWlSurface); 

#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
    void createIviApplicationSurface(QtWaylandServer::ivi_application::Resource *resource, 
                                    uint32_t ivi_id, struct ::wl_resource *surface, uint32_t id);
    QWaylandQuickSurface *fullscreenSurface() const;
    void loadQmlComponent(const QSize &size);
    Q_INVOKABLE QWaylandSurfaceItem *item(QWaylandSurface *surf);
#endif
#if WRSCOMPOSITOR_REARDISPLAY
    void setRearDisplay(QQuickView *);
    QWaylandQuickOutput* mainOutput() { return mMainOutput; };
    QWaylandQuickOutput* rearOutput() { return mRearOutput; };
    void restoreSwappedWindow(QWaylandSurfaceItem *surfaceItem);
signals:
    void swappedWindowRestored(QVariant window);
#endif
private slots:
    void slotFocusObjectChanged(QObject *);
#if WRSCOMPOSITOR_REARDISPLAY
    void slotSwapWindow(const QVariant &v);
    void slotCloneWindow(const QVariant &v);
    void slotCloseClonedWindow(const QVariant &v);
#endif

#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
signals:
    void windowAdded(QVariant surface);
    void windowResized(QVariant surface);
    void fullscreenSurfaceChanged();
    void windowDestroyed(QVariant surface);

public slots:
    //void destroyWindow(QVariant window);
    void setFullscreenSurface(QWaylandQuickSurface *surface);

private slots:
    void surfaceMapped();
    void surfaceUnmapped();
    void surfaceDestroyed();
    void windowPropertyChanged(const QString &property, const QVariant &value);
    void sizeChanged();
    void sendCallbacks();

protected:
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void surfaceCreated(QWaylandSurface *surface);

private:
#if WRSCOMPOSITOR_REARDISPLAY
    RearDisplay                             *mRearDisplay;
    QWaylandQuickOutput                     *mRearOutput;
#endif
    QWaylandQuickOutput                     *mMainOutput;
    QWaylandQuickSurface                    *m_fullscreenSurface;
    WrsIVIModel::IVIScene     *mIviScene;
    QString                                 mProgram;

    WrsIviController                        *mIviController;
    WrsIviApplication                       *mIviApplication;
    WrsIviSurface                           *mIviSurface;
#endif


#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR

#endif
};
#endif
