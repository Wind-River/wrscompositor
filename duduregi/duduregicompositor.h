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
#include <QtCompositor/private/qwayland-server-wayland.h>
#include "qwayland-server-ivi-controller.h"

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
    DuduregiCompositor(const QString &program=QString::null, const QString &display=QString::null);
    ~DuduregiCompositor();

#if DUDUREGI_WAYLAND_COMPOSITOR
    QWaylandQuickSurface *fullscreenSurface() const;
    Q_INVOKABLE QWaylandSurfaceItem *item(QWaylandSurface *surf);
#endif
#if DUDUREGI_REARDISPLAY
    void setRearDisplay(QQuickView *);
    QWaylandQuickOutput* mainOutput() { return mMainOutput; };
    QWaylandQuickOutput* rearOutput() { return mRearOutput; };
    void restoreSwappedWindow(QQuickItem *windowFrame);
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
    void windowAdded(QVariant window);
    void windowResized(QVariant window);
    void fullscreenSurfaceChanged();

public slots:
    void destroyWindow(QVariant window);
    void setFullscreenSurface(QWaylandQuickSurface *surface);

private slots:
    void surfaceMapped();
    void surfaceUnmapped();
    void surfaceDestroyed();
    void sendCallbacks();

protected:
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

private:
    GeniviWaylandIVIExtension::IVISurface* findSurfaceByResource(struct ::wl_resource *rsc);
#endif
};
#endif
