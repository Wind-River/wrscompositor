#include "duduregiconfig.h"
#include "reardisplay.h"
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>

RearDisplay::RearDisplay(QWindow *parent)
    : QQuickView(parent)
{
    setTitle(QLatin1String("Wind River Duduregi Rear Display Example"));
    setSource(QUrl("rearmain.qml"));
    setResizeMode(QQuickView::SizeRootObjectToView);

    QScreen *screen = QGuiApplication::primaryScreen();
    if(QGuiApplication::screens().count() > 1) {
        screen = QGuiApplication::screens().at(3); // third
        setScreen(screen);
    }
    setGeometry(screen->availableGeometry());
}

RearDisplay::~RearDisplay() {
}
