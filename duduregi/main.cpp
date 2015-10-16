#include "duduregiconfig.h"

#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include "duduregicompositor.h"

#if DUDUREGI_WEBENGINE
#include <QtWebEngineWidgets>
#else
#include <QtWebKitWidgets>
#endif

#include "Process.h"
#include "vna_dbusclient.h"
#include "wr_dbusclient.h"
#include "projectionmode.h"

#ifdef DIGITALCLUSTER
#include <QQuickView>
#endif

#if DUDUREGI_WAYLAND_COMPOSITOR
#include "GeniviWaylandIVIExtension.h"
#endif

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
    qmlRegisterType<ProjectionModePrivate>("com.windriver.automotive", 1, 0, "ProjectionMode");
#if DUDUREGI_WAYLAND_COMPOSITOR
    qmlRegisterType<GeniviWaylandIVIExtension::IVIScene>("com.windriver.genivi", 1, 0, "IVIScene");
    qmlRegisterType<GeniviWaylandIVIExtension::IVIScreen>("com.windriver.genivi", 1, 0, "IVIScreen");
    qmlRegisterType<GeniviWaylandIVIExtension::IVILayer>("com.windriver.genivi", 1, 0, "IVILayer");
    qmlRegisterType<GeniviWaylandIVIExtension::IVISurface>("com.windriver.genivi", 1, 0, "IVISurface");
    qRegisterMetaType<GeniviWaylandIVIExtension::IVILayer* >("IVILayer*");
    qRegisterMetaType<GeniviWaylandIVIExtension::IVISurface* >("IVISurface*");
    qRegisterMetaType<GeniviWaylandIVIExtension::IVIScreen* >("IVIScreen*");
    qRegisterMetaType<GeniviWaylandIVIExtension::IVIScene* >("IVIScene*");
#endif

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

    DuduregiCompositor compositor;
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

    return app.exec();
}
