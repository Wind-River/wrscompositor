#ifndef REAR_DISPLAY_H
#define REAR_DISPLAY_H
#include "duduregiconfig.h"

#include <QSurfaceFormat>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QSettings>

#include <QTimer>

#include <QtCompositor/qwaylandsurfaceitem.h>
#include <QtCompositor/qwaylandoutput.h>
#include <QtCompositor/qwaylandquickcompositor.h>
#include <QtCompositor/qwaylandquicksurface.h>
#include <QtCompositor/qwaylandquickoutput.h>
#include <QtCompositor/private/qwlcompositor_p.h>
#include <QtCompositor/private/qwloutput_p.h>
#include <QtCompositor/private/qwayland-server-wayland.h>

class RearDisplay : public QQuickView
{
    Q_OBJECT

public:
    RearDisplay(QWindow *parent = 0);
    virtual ~RearDisplay();
    void addSwappedWindow(QQuickItem *windowFrame);
    void addClonedWindow(QWaylandSurfaceItem *item);
    void closeClonedWindow(QWaylandQuickSurface *surface);
    void setMainDisplay(QQuickView *qv) { mMainDisplay = qv; };
    void setMainOutput(QWaylandQuickOutput *o) { mMainOutput = o; };
signals:
    void windowSwapped(QVariant window);
    void windowCloned(QVariant window);
    void windowCloneClosed(QVariant window);
private slots:
    void slotSwappedWindowRestore(const QVariant &v);
    void slotClonedSurfaceDestroy(const QVariant &v);
private:
    QQuickView *mMainDisplay;
    QWaylandQuickOutput* mMainOutput;
};
#endif
