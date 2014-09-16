/*
 * Copyright © 2014 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include <QtWidgets>
#include "htmlwidget.h"
#include "menuhandler.h"
#include <QQuickView>
#include <QQmlContext>
#include <QQuickItem>

class Home : public QQuickView
{
    Q_OBJECT

public:
    Home() {
        setSource(QUrl("home.qml"));
        setResizeMode(QQuickView::SizeRootObjectToView);
        setColor(Qt::black);
    }
};

int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<MenuHandler>("com.windriver.home", 1, 0, "MenuHandler");

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();

    Home home;
    home.setGeometry(screenGeometry);
    home.show();

    QUrl url;
    url = QUrl("file:///opt/windriver/share/html5apps/WRIPodPlayer/index.html");
    HTMLWidget html(url);
    html.setGeometry(screenGeometry);
    html.hide();

    MenuHandler::mainQmlView = &home;
    MenuHandler::htmlView = &html;

    return app.exec();
}

#include "main.moc"
