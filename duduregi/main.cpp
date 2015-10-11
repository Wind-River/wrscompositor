#include "duduregiconfig.h"
#if DUDUREGI_WAYLAND_COMPOSITOR
#include <QtCompositor/qwaylandsurfaceitem.h>
#include <QtCompositor/qwaylandoutput.h>
#include <QtCompositor/qwaylandquickcompositor.h>
#include <QtCompositor/qwaylandquicksurface.h>
#include <QtCompositor/qwaylandquickoutput.h>
#include <QtCompositor/private/qwlcompositor_p.h>
#include "qwayland-server-ivi-controller.h"
#endif

#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

#if DUDUREGI_WEBENGINE
#include <QtWebEngineWidgets>
#else
#include <QtWebKitWidgets>

#endif

#include <QScreen>
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>
#include <QScreen>
#include <QMainWindow>



#include "Process.h"
#include "vna_dbusclient.h"
#include "wr_dbusclient.h"

class QmlCompositor : public QQuickView
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
    QmlCompositor()
#if DUDUREGI_WAYLAND_COMPOSITOR
        : QWaylandQuickCompositor(0, DefaultExtensions | SubSurfaceExtension)
        , QtWaylandServer::ivi_controller_surface(QWaylandCompositor::handle()->wl_display(), 1)
        , QtWaylandServer::ivi_controller_layer(QWaylandCompositor::handle()->wl_display(), 1)
        , QtWaylandServer::ivi_controller_screen(QWaylandCompositor::handle()->wl_display(), 1)
        , QtWaylandServer::ivi_controller(QWaylandCompositor::handle()->wl_display(), 1)
              , m_fullscreenSurface(0)

#endif
    {
        setSource(QUrl("main.qml"));
        setResizeMode(QQuickView::SizeRootObjectToView);
        setColor(Qt::black);
        winId();
#if DUDUREGI_WAYLAND_COMPOSITOR
        addDefaultShell();
        createOutput(this, DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME);
        setClientFullScreenHint(true);
        connect(this, SIGNAL(afterRendering()), this, SLOT(sendCallbacks()));

#endif
    }
    ~QmlCompositor() {
        QSettings settings(DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME);
        //settings.setValue("size", size());
        //settings.setValue("windowState", windowState());
        settings.setValue("position", position());
    }

#if DUDUREGI_WAYLAND_COMPOSITOR
    QWaylandQuickSurface *fullscreenSurface() const
    {
        return m_fullscreenSurface;
    }

    Q_INVOKABLE QWaylandSurfaceItem *item(QWaylandSurface *surf)
    {
        return static_cast<QWaylandSurfaceItem *>(surf->views().first());
    }

#endif

#if DUDUREGI_WAYLAND_COMPOSITOR
signals:
    void windowAdded(QVariant window);
    void windowResized(QVariant window);
    void fullscreenSurfaceChanged();

public slots:
    void destroyWindow(QVariant window) {
        qvariant_cast<QObject *>(window)->deleteLater();
    }

    void setFullscreenSurface(QWaylandQuickSurface *surface) {
        if (surface == m_fullscreenSurface)
            return;
        m_fullscreenSurface = surface;
        emit fullscreenSurfaceChanged();
    }

private slots:
    void surfaceMapped() {
        QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());
        emit windowAdded(QVariant::fromValue(surface));
    }
    void surfaceUnmapped() {
        QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());
        if (surface == m_fullscreenSurface)
            m_fullscreenSurface = 0;
    }

    void surfaceDestroyed() {
        QWaylandQuickSurface *surface = static_cast<QWaylandQuickSurface *>(sender());
        if (surface == m_fullscreenSurface)
            m_fullscreenSurface = 0;
    }

    void sendCallbacks() {
        if (m_fullscreenSurface)
            sendFrameCallbacks(QList<QWaylandSurface *>() << m_fullscreenSurface);
        else
            sendFrameCallbacks(surfaces());
    }

protected:
    void resizeEvent(QResizeEvent *event)
    {
        QQuickView::resizeEvent(event);
        QWaylandCompositor::setOutputGeometry(QRect(0, 0, width(), height()));
    }

    void surfaceCreated(QWaylandSurface *surface) {
        connect(surface, SIGNAL(surfaceDestroyed()), this, SLOT(surfaceDestroyed()));
        connect(surface, SIGNAL(mapped()), this, SLOT(surfaceMapped()));
        connect(surface,SIGNAL(unmapped()), this,SLOT(surfaceUnmapped()));
    }

private:
    QWaylandQuickSurface *m_fullscreenSurface;
#endif


#if DUDUREGI_WAYLAND_COMPOSITOR
protected:
    void ivi_controller_surface_bind_resource(QtWaylandServer::ivi_controller_surface::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_surface_destroy_resource(QtWaylandServer::ivi_controller_surface::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_surface_set_visibility(QtWaylandServer::ivi_controller_surface::Resource *resource, uint32_t visibility) {
        (void)resource;
        qDebug() << __func__ << visibility;
    };
    void ivi_controller_surface_set_opacity(QtWaylandServer::ivi_controller_surface::Resource *resource, wl_fixed_t opacity) {
        (void)resource;
        qDebug() << __func__ << opacity;
    };
    void ivi_controller_surface_set_source_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
        (void)resource;
        qDebug() << __func__ << x << y << width << height;
    };
    void ivi_controller_surface_set_destination_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
        (void)resource;
        qDebug() << __func__ << x << y << width << height;
    };
    void ivi_controller_surface_set_configuration(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t width, int32_t height) {
        (void)resource;
        qDebug() << __func__ << width << height;
    };
    void ivi_controller_surface_set_orientation(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t orientation) {
        (void)resource;
        qDebug() << __func__ << orientation;
    };
    void ivi_controller_surface_screenshot(QtWaylandServer::ivi_controller_surface::Resource *resource, const QString &filename) {
        (void)resource;
        qDebug() << __func__ << filename;
    };
    void ivi_controller_surface_dosend_stats(QtWaylandServer::ivi_controller_surface::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_surface_destroy(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t destroy_scene_object) {
        (void)resource;
        qDebug() << __func__ << destroy_scene_object;
    };



    void ivi_controller_layer_bind_resource(QtWaylandServer::ivi_controller_layer::Resource *resource) {
        (void)resource;
        qDebug() << __func__ ;
    };
    void ivi_controller_layer_destroy_resource(QtWaylandServer::ivi_controller_layer::Resource *resource) {
        (void)resource;
        qDebug() << __func__ ;
    };
    void ivi_controller_layer_set_visibility(QtWaylandServer::ivi_controller_layer::Resource *resource, uint32_t visibility) {
        (void)resource;
        qDebug() << __func__ << visibility;
    };
    void ivi_controller_layer_set_opacity(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_fixed_t opacity) {
        (void)resource;
        qDebug() << __func__ << opacity;
    };
    void ivi_controller_layer_set_source_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
        (void)resource;
        qDebug() << __func__ << x << y << width << height;
    };
    void ivi_controller_layer_set_destination_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
        (void)resource;
        qDebug() << __func__ << x << y << width << height;
    };
    void ivi_controller_layer_set_configuration(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t width, int32_t height) {
        (void)resource;
        qDebug() << __func__ << width << height;
    };
    void ivi_controller_layer_set_orientation(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t orientation) {
        (void)resource;
        qDebug() << __func__ << orientation;
    };
    void ivi_controller_layer_screenshot(QtWaylandServer::ivi_controller_layer::Resource *resource, const QString &filename) {
        (void)resource;
        qDebug() << __func__ << filename;
    };
    void ivi_controller_layer_clear_surfaces(QtWaylandServer::ivi_controller_layer::Resource *resource) {
        (void)resource;
    };
    void ivi_controller_layer_add_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface) {
        (void)resource;
        qDebug() << __func__ << surface;
    };
    void ivi_controller_layer_remove_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface) {
        (void)resource;
        qDebug() << __func__ << surface;
    };
    void ivi_controller_layer_set_render_order(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_array *id_surfaces) {
        (void)resource;
        (void)id_surfaces;
    };
    void ivi_controller_layer_destroy(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t destroy_scene_object) {
        (void)resource;
        qDebug() << __func__ << destroy_scene_object;
    };



    void ivi_controller_screen_bind_resource(QtWaylandServer::ivi_controller_screen::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_screen_destroy_resource(QtWaylandServer::ivi_controller_screen::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_screen_destroy(QtWaylandServer::ivi_controller_screen::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_screen_clear(QtWaylandServer::ivi_controller_screen::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_screen_add_layer(QtWaylandServer::ivi_controller_screen::Resource *resource, struct ::wl_resource *layer) {
        (void)resource;
        qDebug() << __func__ << layer;
    };
    void ivi_controller_screen_screenshot(QtWaylandServer::ivi_controller_screen::Resource *resource, const QString &filename) {
        (void)resource;
        qDebug() << __func__ << filename;
    };
    void ivi_controller_screen_set_render_order(QtWaylandServer::ivi_controller_screen::Resource *resource, wl_array *id_layers) {
        (void)resource;
        qDebug() << __func__ << id_layers;
    };





    void ivi_controller_bind_resource(QtWaylandServer::ivi_controller::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_destroy_resource(QtWaylandServer::ivi_controller::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_commit_changes(QtWaylandServer::ivi_controller::Resource *resource) {
        (void)resource;
        qDebug() << __func__;
    };
    void ivi_controller_layer_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_layer, int32_t width, int32_t height, uint32_t id) {
        (void)resource;
        qDebug() << __func__ << id_layer << width << height << id;
    };
    void ivi_controller_surface_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_surface, uint32_t id) {
        (void)resource;
        qDebug() << __func__ << id_surface << id;
    };
#endif
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
#if 0
    QDesktopWidget d;
    QRect screenGeometry = d.screenGeometry();
#endif

    qmlRegisterType<Process>("com.windriver.duduregi", 1, 0, "Process");
    qmlRegisterType<VNADBusClient>("com.windriver.automotive", 1, 0, "VNADBusClient");
    qmlRegisterType<WRDBusClient>("com.windriver.automotive", 1, 0, "WRDBusClient");

#if DUDUREGI_WEBENGINE
    QtWebEngine::initialize();
#endif

#ifdef DIGITALCLUSTER
    QQuickView kv;
    kv.setSource(QUrl("cluster.qml"));
    kv.setResizeMode(QQuickView::SizeRootObjectToView);
    kv.setScreen(QGuiApplication::screens().at(1));
    kv.setGeometry(screenGeometry);
    kv.show();
#endif

    QmlCompositor compositor;
#if DUDUREGI_WAYLAND_COMPOSITOR
    compositor.setTitle(QLatin1String("Wind River Duduregi Wayland Compositor"));
#endif
    if(app.arguments().contains("--720")||app.arguments().contains("--1080")) {
        if(app.arguments().contains("--720"))
            compositor.setGeometry(50, 50, 1280, 720);
        else
            compositor.setGeometry(50, 50, 1920, 1080);
        QSettings settings(DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME);
        compositor.setPosition(settings.value("position").toPoint());
    } else
        compositor.setGeometry(screenGeometry);
    compositor.show();

    compositor.rootContext()->setContextProperty("compositor", &compositor);


#if DUDUREGI_WAYLAND_COMPOSITOR
    QObject::connect(&compositor, SIGNAL(windowAdded(QVariant)), compositor.rootObject(), SLOT(windowAdded(QVariant)));
    QObject::connect(&compositor, SIGNAL(windowResized(QVariant)), compositor.rootObject(), SLOT(windowResized(QVariant)));
#endif

    return app.exec();
}

#include "main.moc"
