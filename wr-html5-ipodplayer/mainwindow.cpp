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

    button = new QPushButton(tr("Back"), this);
    connect(button, SIGNAL(clicked()), SLOT(backClicked()));
}

void MainWindow::setProgress(int p)
{
    progress = p;
}
void MainWindow::finishLoading(bool)
{
    progress = 100;
}

void MainWindow::backClicked()
{
    const char *cmd = "/opt/LGDemo/ipod_stop.sh";
    QProcess::execute(cmd);
    QCoreApplication::instance()->quit();
}
void MainWindow::resizeEvent(QResizeEvent * event) {
    view->resize(event->size());

    // to place the button on right position
    int buttonMargin = 50;
    button->move(event->size().width() - button->size().width() - buttonMargin,
                buttonMargin);
}
