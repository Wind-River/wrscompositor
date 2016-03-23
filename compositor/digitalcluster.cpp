/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include "duduregiconfig.h"
#include "digitalcluster.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>

DigitalCluster::DigitalCluster(QWindow *parent)
    : QQuickView(parent)
{
    setTitle(QLatin1String("Wind River Duduregi Digital Cluster Example"));
    QUrl programUrl = QUrl("qrc:///cluster.qml");
    if(qApp->arguments().contains("--debug"))
        programUrl = QUrl("hmi/" DUDUREGI_HMI_PROFILE "/cluster.qml");
    setSource(programUrl);
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
