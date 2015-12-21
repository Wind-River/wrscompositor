#include "duduregiconfig.h"
#include "digitalcluster.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>

DigitalCluster::DigitalCluster(QWindow *parent)
    : QQuickView(parent)
{
    setTitle(QLatin1String("Wind River Duduregi Digital Cluster Example"));
    setSource(QUrl("cluster.qml"));
    setResizeMode(QQuickView::SizeRootObjectToView);

    QScreen *screen = QGuiApplication::primaryScreen();
    if(QGuiApplication::screens().count() > 1) {
        screen = QGuiApplication::screens().at(1); // secondary
        setScreen(screen);
    }
    setGeometry(screen->availableGeometry());
}

DigitalCluster::~DigitalCluster() {
}