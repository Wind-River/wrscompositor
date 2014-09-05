/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QtWidgets>
#include "mainwindow.h"

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);
    QUrl url;
    /*
    if (argc > 1)
        url = QUrl::fromUserInput(argv[1]);
    else
    */
    url = QUrl("file:///opt/windriver/share/html5apps/WRIPodPlayer/index.html");

    MainWindow *browser = new MainWindow(url);
    if(argc==2 && !strcmp(argv[1], "--fullscreen"))
        browser->showFullScreen();
    else {
        browser->resize(800,400);
        browser->show();
    }
    return app.exec();
}
