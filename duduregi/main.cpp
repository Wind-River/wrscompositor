#include "duduregiconfig.h"
#if DUDUREGI_WAYLAND_COMPOSITOR
#include <QtCompositor/qwaylandsurfaceitem.h>
#include <QtCompositor/qwaylandoutput.h>
#include <QtCompositor/qwaylandquickcompositor.h>
#include <QtCompositor/qwaylandquicksurface.h>
#include <QtCompositor/qwaylandquickoutput.h>
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
    compositor.setTitle(QLatin1String("QML Compositor"));
#endif
    if(app.arguments().contains("--720")) {
        compositor.setGeometry(50, 50, 1280, 720);
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
