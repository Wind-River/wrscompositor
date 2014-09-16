/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QtWidgets>

#include "menuhandler.h"
#include "mainwidget.h"

MainWidget::MainWidget()
    : QWidget(NULL)
{
    qml = new QMLWidget(this);
    QWidget *container = QWidget::createWindowContainer(qml, this);
    container->show();

    QUrl url;
    url = QUrl("file:///opt/windriver/share/html5apps/WRIPodPlayer/index.html");
    html = new HTMLWidget (url, this);
    html->hide();

    MenuHandler::mainQmlView = container;
    MenuHandler::htmlView = html;
}
void MainWidget::resizeEvent(QResizeEvent * event) {
    qml->resize(event->size());
    html->resize(event->size());
}

