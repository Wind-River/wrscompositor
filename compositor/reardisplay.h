/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef REAR_DISPLAY_H
#define REAR_DISPLAY_H
#include "config.h"

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
