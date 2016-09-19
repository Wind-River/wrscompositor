/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#ifndef DUDUREGI_COMPOSITOR
#define DUDUREGI_COMPOSITOR
#include "duduregiconfig.h"
#if DUDUREGI_WAYLAND_COMPOSITOR
#include <QtCompositor/qwaylandsurfaceitem.h>
#include <QtCompositor/qwaylandoutput.h>
#include <QtCompositor/qwaylandquickcompositor.h>
#include <QtCompositor/qwaylandquicksurface.h>
#include <QtCompositor/qwaylandquickoutput.h>
#include <QtCompositor/private/qwlcompositor_p.h>
#include <QtCompositor/private/qwloutput_p.h>
#include <QtCompositor/private/qwlinputdevice_p.h>
#include <QtCompositor/private/qwayland-server-wayland.h>
#include "qwayland-server-ivi-controller.h"
#include "qwayland-server-ivi-application.h"

#include "GeniviWaylandIVIExtension.h"
#endif

#include <QSurfaceFormat>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QSettings>

#include <QTimer>
#if DUDUREGI_REARDISPLAY
#include "reardisplay.h"
#endif

class DuduregiCompositor : public QQuickView
#if DUDUREGI_WAYLAND_COMPOSITOR
                      , public QWaylandQuickCompositor
                      , public QtWaylandServer::ivi_surface
                      , public QtWaylandServer::ivi_application
                      , public QtWaylandServer::ivi_controller_surface
                      , public QtWaylandServer::ivi_controller_layer
                      , public QtWaylandServer::ivi_controller_screen
                      , public QtWaylandServer::ivi_controller
#endif
{
    Q_OBJECT
#if DUDUREGI_WAYLAND_COMPOSITOR
    Q_PROPERTY(QWaylandQuickSurface* fullscreenSurface READ fullscreenSurface WRITE setFullscreenSurface NOTIFY fullscreenSurfaceChanged)
#endif

public:
    DuduregiCompositor(const QString &display=QString::null, const QString &program=QString::null);
    ~DuduregiCompositor();

#if DUDUREGI_WAYLAND_COMPOSITOR
    QWaylandQuickSurface *fullscreenSurface() const;
    void loadQmlComponent(const QSize &size);
    Q_INVOKABLE QWaylandSurfaceItem *item(QWaylandSurface *surf);
    Q_INVOKABLE QString getProcessNameByPid(int pid); 
#endif
#if DUDUREGI_REARDISPLAY
    void setRearDisplay(QQuickView *);
    QWaylandQuickOutput* mainOutput() { return mMainOutput; };
    QWaylandQuickOutput* rearOutput() { return mRearOutput; };
    void restoreSwappedWindow(QWaylandSurfaceItem *surfaceItem);
signals:
    void swappedWindowRestored(QVariant window);
#endif
private slots:
    void slotFocusObjectChanged(QObject *);
#if DUDUREGI_REARDISPLAY
    void slotSwapWindow(const QVariant &v);
    void slotCloneWindow(const QVariant &v);
    void slotCloseClonedWindow(const QVariant &v);
#endif

#if DUDUREGI_WAYLAND_COMPOSITOR
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
    void sendCallbacks();

protected:
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void surfaceCreated(QWaylandSurface *surface);

private:
#if DUDUREGI_REARDISPLAY
    RearDisplay *mRearDisplay;
    QWaylandQuickOutput *mRearOutput;
#endif
    QWaylandQuickOutput *mMainOutput;
    QWaylandQuickSurface *m_fullscreenSurface;
    GeniviWaylandIVIExtension::IVIScene *mGeniviExt;
    QString mProgram;
#endif


#if DUDUREGI_WAYLAND_COMPOSITOR
protected:
    void ivi_controller_surface_bind_resource(QtWaylandServer::ivi_controller_surface::Resource *resource);
    void ivi_controller_surface_destroy_resource(QtWaylandServer::ivi_controller_surface::Resource *resource);
    void ivi_controller_surface_set_visibility(QtWaylandServer::ivi_controller_surface::Resource *resource, uint32_t visibility);
    void ivi_controller_surface_set_opacity(QtWaylandServer::ivi_controller_surface::Resource *resource, wl_fixed_t opacity);
    void ivi_controller_surface_set_source_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);
    void ivi_controller_surface_set_destination_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);
    void ivi_controller_surface_set_configuration(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t width, int32_t height);
    void ivi_controller_surface_set_orientation(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t orientation);
    void ivi_controller_surface_screenshot(QtWaylandServer::ivi_controller_surface::Resource *resource, const QString &filename);
    void ivi_controller_surface_dosend_stats(QtWaylandServer::ivi_controller_surface::Resource *resource);
    void ivi_controller_surface_destroy(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t destroy_scene_object);

    void ivi_controller_layer_bind_resource(QtWaylandServer::ivi_controller_layer::Resource *resource);
    void ivi_controller_layer_destroy_resource(QtWaylandServer::ivi_controller_layer::Resource *resource);
    void ivi_controller_layer_set_visibility(QtWaylandServer::ivi_controller_layer::Resource *resource, uint32_t visibility);
    void ivi_controller_layer_set_opacity(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_fixed_t opacity);
    void ivi_controller_layer_set_source_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);
    void ivi_controller_layer_set_destination_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);
    void ivi_controller_layer_set_configuration(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t width, int32_t height);
    void ivi_controller_layer_set_orientation(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t orientation);
    void ivi_controller_layer_screenshot(QtWaylandServer::ivi_controller_layer::Resource *resource, const QString &filename);
    void ivi_controller_layer_clear_surfaces(QtWaylandServer::ivi_controller_layer::Resource *resource);
    void ivi_controller_layer_add_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface);
    void ivi_controller_layer_remove_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface);
    void ivi_controller_layer_set_render_order(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_array *id_surfaces);
    void ivi_controller_layer_destroy(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t destroy_scene_object);

    void ivi_controller_screen_bind_resource(QtWaylandServer::ivi_controller_screen::Resource *resource);
    void ivi_controller_screen_destroy_resource(QtWaylandServer::ivi_controller_screen::Resource *resource);
    void ivi_controller_screen_destroy(QtWaylandServer::ivi_controller_screen::Resource *resource);
    void ivi_controller_screen_clear(QtWaylandServer::ivi_controller_screen::Resource *resource);
    void ivi_controller_screen_add_layer(QtWaylandServer::ivi_controller_screen::Resource *resource, struct ::wl_resource *layer);
    void ivi_controller_screen_screenshot(QtWaylandServer::ivi_controller_screen::Resource *resource, const QString &filename);
    void ivi_controller_screen_set_render_order(QtWaylandServer::ivi_controller_screen::Resource *resource, wl_array *id_layers);

    void ivi_controller_bind_resource(QtWaylandServer::ivi_controller::Resource *resource);
    void ivi_controller_destroy_resource(QtWaylandServer::ivi_controller::Resource *resource);
    void ivi_controller_commit_changes(QtWaylandServer::ivi_controller::Resource *resource);
    void ivi_controller_layer_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_layer, int32_t width, int32_t height, uint32_t id);
    void ivi_controller_surface_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_surface, uint32_t id);

    void ivi_surface_bind_resource(QtWaylandServer::ivi_surface::Resource *resource);
    void ivi_surface_destroy_resource(QtWaylandServer::ivi_surface::Resource *resource);
    void ivi_surface_destroy(QtWaylandServer::ivi_surface::Resource *resource);

    void ivi_application_bind_resource(QtWaylandServer::ivi_application::Resource *);
    void ivi_application_destroy_resource(QtWaylandServer::ivi_application::Resource *);
    void ivi_application_surface_create(QtWaylandServer::ivi_application::Resource *, uint32_t ivi_id, struct ::wl_resource *surface, uint32_t id);
private:
    GeniviWaylandIVIExtension::IVISurface* findSurfaceByResource(struct ::wl_resource *rsc);
#endif
};
#endif
