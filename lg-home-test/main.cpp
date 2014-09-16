/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QtWidgets>
#include "mainwidget.h"
#include "menuhandler.h"
#include <QQuickView>
#include <QQuickItem>

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<MenuHandler>("com.windriver.home", 1, 0, "MenuHandler");

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    MainWidget mw;
    mw.setGeometry(screenGeometry);
    mw.show();


    return app.exec();
}
