/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QtWidgets>
#include <QtNetwork>
#include <QtWebKitWidgets>
#include "mainwindow.h"

MainWindow::MainWindow(const QUrl& url)
{
    progress = 0;

    QNetworkProxyFactory::setUseSystemConfiguration(true);

    view = new QWebView(this);
    view->load(url);
    connect(view, SIGNAL(loadProgress(int)), SLOT(setProgress(int)));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
    setCentralWidget(view);
    setWindowTitle(tr("WindRiver HTML5 iPod Player Sample"));
}

void MainWindow::setProgress(int p)
{
    progress = p;
}
void MainWindow::finishLoading(bool)
{
    progress = 100;
}
