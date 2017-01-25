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

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickView>
#include "qwayland-common.h"

class Keyboard : public QQuickView,
                 public QtWaylandClient::QWaylandCommon
{
public:
    Keyboard(uint32_t role);
    ~Keyboard();

protected:
    void configureIviSurface(QWindow *window, int width, int height);
    bool eventFilter(QObject *obj, QEvent *event);
};

Keyboard::Keyboard(uint32_t role)
    :  QQuickView(QUrl("qrc:///InputPanel.qml")), QWaylandCommon(role) {
    installEventFilter(this);
}

Keyboard::~Keyboard()
{

}

void Keyboard::configureIviSurface(QWindow *window, int width, int height) {
    QQuickView *view = static_cast<QQuickView *>(window);
    if (view != NULL) {
        QQuickItem *object = view->rootObject();
        if (object) {
            qDebug() << "Keyboard::configureIviSurface, configure QQuickItem size " << width << "," << height;
            object->setWidth(width);
            object->setHeight(height);
        }
    }

    QWaylandCommon::configureIviSurface(window, width, height);
}

bool Keyboard::eventFilter(QObject *obj, QEvent *event) {
    //qDebug() << "Keyboard::eventFilter, event " << event;
    switch (event->type()) {
        case QEvent::Close:
        {
            qDebug() << "Keyboard has closed";
            break;
        }

        case QEvent::PlatformSurface:
        {
            QPlatformSurfaceEvent::SurfaceEventType eventType =
                static_cast<QPlatformSurfaceEvent *>(event)->surfaceEventType();

            if (eventType == QPlatformSurfaceEvent::SurfaceCreated) {
                qDebug() << "Keyboard has created surface";
                this->createIviSurface(this);

                struct ::wl_surface *surface = this->getWaylandSurface(this);
                QtWayland::wl_input_panel *inputpanel =
                                this->getWaylandInputPanel();

                struct ::wl_input_panel_surface *ips =
                                inputpanel->get_input_panel_surface(surface);
                struct ::wl_output *output =
                                this->getWaylandOutput();

                wl_input_panel_surface_set_toplevel(ips,
                                output, WL_INPUT_PANEL_SURFACE_POSITION_CENTER_BOTTOM);
            } else if (eventType == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed) {
                qDebug() << "Keyboard has destroyed surface";
            }
            break;
        }

        default:
            break;
    }

    return QObject::eventFilter(obj, event);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    uint32_t role = QtWaylandClient::WRS_IVI_ID_SURFACE_KEYBOARD;

    Keyboard keyboard(role);
    keyboard.show();

    return app.exec();
}