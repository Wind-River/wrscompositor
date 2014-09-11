#ifdef QT_COMPOSITOR_QUICK_LIB
#include "qwaylandcompositor.h"
#include "qwaylandsurface.h"
#include "qwaylandsurfaceitem.h"
#endif

#include <QGuiApplication>
#include <QSurfaceFormat>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>

#ifdef QT_WEBENGINEWIDGETS_LIB
#include <QtWebEngineWidgets>
#else
#include <QtWebKitWidgets>

#define DeclarativeView QQuickView
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

class QmlCompositor : public DeclarativeView
#ifdef QT_COMPOSITOR_QUICK_LIB
                      , public QWaylandCompositor
#endif
{
    Q_OBJECT
#ifdef QT_COMPOSITOR_QUICK_LIB
    Q_PROPERTY(QWaylandSurface* fullscreenSurface READ fullscreenSurface WRITE setFullscreenSurface NOTIFY fullscreenSurfaceChanged)
#endif

public:
    QmlCompositor()
#ifdef QT_COMPOSITOR_QUICK_LIB
        : QWaylandCompositor(this, 0, static_cast<ExtensionFlag>(DefaultExtensions | SubSurfaceExtension)), m_fullscreenSurface(0)
#endif
    {
        setSource(QUrl("main.qml"));
        setResizeMode(DeclarativeView::SizeRootObjectToView);
        setColor(Qt::black);
        winId();
#ifdef QT_COMPOSITOR_QUICK_LIB
        setClientFullScreenHint(true);
	connect(this, SIGNAL(frameSwapped()), this, SLOT(frameSwappedSlot()));
#endif
    }

#ifdef QT_COMPOSITOR_QUICK_LIB
    QWaylandSurface *fullscreenSurface() const
    {
        return m_fullscreenSurface;
    }
    Q_INVOKABLE bool configure(QWaylandSurface *surface, int width, int height)
    {
        surface->requestSize(QSize(width, height));
        return true;
    }
#endif

signals:
    void windowAdded(QVariant window);
    void windowDestroyed(QVariant window);
    void windowResized(QVariant window);
    void fullscreenSurfaceChanged();

public slots:
    void destroyWindow(QVariant window) {
        qvariant_cast<QObject *>(window)->deleteLater();
    }

#ifdef QT_COMPOSITOR_QUICK_LIB
    void destroyClientForWindow(QVariant window) {
        QWaylandSurface *surface = qobject_cast<QWaylandSurfaceItem *>(qvariant_cast<QObject *>(window))->surface();
        destroyClientForSurface(surface);
    }

    void setFullscreenSurface(QWaylandSurface *surface) {
        if (surface == m_fullscreenSurface)
            return;
        m_fullscreenSurface = surface;
        emit fullscreenSurfaceChanged();
    }

private slots:
    void surfaceMapped() {
        QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
        //Ignore surface if it's not a window surface
        if (!surface->hasShellSurface())
            return;

        QWaylandSurfaceItem *item = surface->surfaceItem();
        //Create a WaylandSurfaceItem if we have not yet
        if (!item)
            item = new QWaylandSurfaceItem(surface, rootObject());

        item->setTouchEventsEnabled(true);
        //item->takeFocus();
        emit windowAdded(QVariant::fromValue(static_cast<QQuickItem *>(item)));
    }
    void surfaceUnmapped() {
        QWaylandSurface *surface = qobject_cast<QWaylandSurface *>(sender());
        if (surface == m_fullscreenSurface)
            m_fullscreenSurface = 0;
        QQuickItem *item = surface->surfaceItem();
        emit windowDestroyed(QVariant::fromValue(item));
    }

    void surfaceDestroyed(QObject *object) {
        QWaylandSurface *surface = static_cast<QWaylandSurface *>(object);
        if (surface == m_fullscreenSurface)
            m_fullscreenSurface = 0;
        QQuickItem *item = surface->surfaceItem();
        emit windowDestroyed(QVariant::fromValue(item));
    }

    void frameSwappedSlot() {
        frameFinished(m_fullscreenSurface);
    }

protected:
    void resizeEvent(QResizeEvent *event)
    {
        DeclarativeView::resizeEvent(event);
        QWaylandCompositor::setOutputGeometry(QRect(0, 0, width(), height()));
    }

    void surfaceCreated(QWaylandSurface *surface) {
        connect(surface, SIGNAL(destroyed(QObject *)), this, SLOT(surfaceDestroyed(QObject *)));
        connect(surface, SIGNAL(mapped()), this, SLOT(surfaceMapped()));
        connect(surface,SIGNAL(unmapped()), this,SLOT(surfaceUnmapped()));
    }

private:
    QWaylandSurface *m_fullscreenSurface;
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

#ifdef QT_WEBENGINEWIDGETS_LIB
    QWebEngine::initialize();
#endif

#ifdef DIGITALCLUSTER
    DeclarativeView kv;
    kv.setSource(QUrl("cluster.qml"));
    kv.setResizeMode(DeclarativeView::SizeRootObjectToView);
    kv.setScreen(QGuiApplication::screens().at(1));
    kv.setGeometry(screenGeometry);
    kv.show();
#endif

    QmlCompositor compositor;
#ifdef QT_COMPOSITOR_QUICK_LIB
    compositor.setTitle(QLatin1String("QML Compositor"));
    compositor.setScreen(QGuiApplication::screens().at(0));
    compositor.rootContext()->setContextProperty("compositor", &compositor);
#endif
    compositor.setGeometry(screenGeometry);


#ifdef QT_COMPOSITOR_QUICK_LIB
    QObject::connect(&compositor, SIGNAL(windowAdded(QVariant)), compositor.rootObject(), SLOT(windowAdded(QVariant)));
    QObject::connect(&compositor, SIGNAL(windowDestroyed(QVariant)), compositor.rootObject(), SLOT(windowDestroyed(QVariant)));
    QObject::connect(&compositor, SIGNAL(windowResized(QVariant)), compositor.rootObject(), SLOT(windowResized(QVariant)));
#endif
    compositor.show();

    return app.exec();
}

#include "main.moc"
