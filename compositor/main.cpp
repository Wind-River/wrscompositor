/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include "duduregiconfig.h"

#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include "duduregicompositor.h"

#if DUDUREGI_WEBENGINE
#include <QtWebEngine>
#else
#include <QtWebKitWidgets>
#endif

#include "Process.h"
#include "vna_dbusclient.h"
#include "wr_dbusclient.h"
#include "projectionmode.h"

#if DUDUREGI_DIGITALCLUSTER
#include "digitalcluster.h"
#endif
#if DUDUREGI_REARDISPLAY
#include "reardisplay.h"
#endif

#if DUDUREGI_WAYLAND_COMPOSITOR
#include "GeniviWaylandIVIExtension.h"
#endif

#define HEIGHT_FULLSCREEN 0xFFFF
#define HEIGHT_720 720
#define HEIGHT_1080 1080

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    int mode = HEIGHT_FULLSCREEN;
    QStringList args = app.arguments();
    QSettings settings(DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME);

    if(args.contains("--720")) {
        mode = HEIGHT_720;
    } else if(args.contains("--1080")) {
        mode = HEIGHT_1080;
    } else if(args.contains("--list-displays")) {
        for(int i=0; i<QGuiApplication::screens().count(); i++) {
            QScreen *screen = QGuiApplication::screens().at(i);
            qDebug() << i << screen->name() << screen->size();
        }
        return 0;
    } else if(app.arguments().contains("--clean-geometry")) {
        settings.clear();
        settings.sync();
        qCritical() << "Geometry Cache Cleared";
        return 0;
    } else if(app.arguments().contains("--help")) {
        qCritical() << "Wind River Duduregi Wayland Compositor";
        qCritical() << "Usage:";
        qCritical() << "  " << app.arguments().at(0) << " [arguments]";
        qCritical() << "";
        qCritical() << "Available Arguments";
        qCritical() << "  --help            Show this help";
        qCritical() << "  --debug           Load QML files from directory";
        qCritical() << "  --720             Show main window as 720px height";
        qCritical() << "  --1080            Show main window as 1080px height";
        qCritical() << "  --clean-geometry  Clean saved window geometry";
        qCritical() << "  --list-displays   Show display list";
        return 0;
    }

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
#if 0
    QDesktopWidget d;
    QRect screenGeometry = d.screenGeometry();
#endif

    qmlRegisterType<Process>("com.windriver.duduregi", 1, 0, "Process");
    qmlRegisterType<VNADBusClient>("com.windriver.automotive", 1, 0, "VNADBusClient");
    qmlRegisterType<WRDBusClient>("com.windriver.automotive", 1, 0, "WRDBusClient");
    qmlRegisterType<ProjectionMode>("com.windriver.automotive", 1, 0, "ProjectionMode");
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

    DuduregiCompositor compositor;

#if DUDUREGI_DIGITALCLUSTER
    DigitalCluster dc;
#endif
#if DUDUREGI_REARDISPLAY
    RearDisplay rd;
    compositor.setRearDisplay(&rd);
    rd.setMainDisplay(&compositor);
    rd.setMainOutput(compositor.mainOutput());
#endif

    if(mode != HEIGHT_FULLSCREEN) {
        if(mode == HEIGHT_720) {
            compositor.setGeometry(settings.value("geometry-for-maindisplay", QRect(50, 50, 1280, 720)).toRect());
#if DUDUREGI_DIGITALCLUSTER
            dc.setGeometry(settings.value("geometry-for-cluster", QRect(100, 100, 1280, 720)).toRect());
#endif
#if DUDUREGI_REARDISPLAY
            rd.setGeometry(settings.value("geometry-for-reardisplay", QRect(150, 150, 1280, 720)).toRect());
#endif
        } else {
            compositor.setGeometry(settings.value("geometry-for-maindisplay", QRect(50, 50, 1920, 1080)).toRect());
#if DUDUREGI_DIGITALCLUSTER
            dc.setGeometry(settings.value("geometry-for-cluster", QRect(100, 100, 1920, 1080)).toRect());
#endif
#if DUDUREGI_REARDISPLAY
            rd.setGeometry(settings.value("geometry-for-reardisplay", QRect(150, 150, 1920, 1080)).toRect());
#endif
        }
    } else { // full screen
        compositor.setGeometry(screenGeometry);
    }
#if DUDUREGI_DIGITALCLUSTER
    dc.show();
#endif
#if DUDUREGI_REARDISPLAY
    rd.show();
#endif
    compositor.show();
    //DuduregiCompositor rearcompositor("cluster.qml", "wayland-1");
    //rearcompositor.show();

    int ret = app.exec();

    if(mode != HEIGHT_FULLSCREEN) { // save last geometry
        settings.setValue("geometry-for-maindisplay", compositor.geometry());
#if DUDUREGI_DIGITALCLUSTER
        settings.setValue("geometry-for-cluster", dc.geometry());
#endif
#if DUDUREGI_REARDISPLAY
        settings.setValue("geometry-for-reardisplay", rd.geometry());
#endif
    }

    return ret;
}