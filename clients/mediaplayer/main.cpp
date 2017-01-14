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

#include <QtCore/QStandardPaths>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickView>
#include "qwaylandiviextension.h"

class MediaPlayer : public QQuickView, public QtWaylandClient::QWaylandIviExtension
{
public:
    MediaPlayer(QWindow* parent = 0);
    ~MediaPlayer();

protected:
    void surfaceConfigure(QWindow *window, int width, int height);
    bool eventFilter(QObject *obj, QEvent *event);
};

MediaPlayer::MediaPlayer(QWindow * parent)
    :  QQuickView(parent), QWaylandIviExtension() {
    setSource(QUrl("qrc:///main.qml"));
    installEventFilter(this);
}

MediaPlayer::~MediaPlayer()
{

}

void MediaPlayer::surfaceConfigure(QWindow *window, int width, int height) {
    qDebug() << __func__ << __LINE__;

    QtWaylandClient::QWaylandWindow *qWaylandWindow =
                            (QtWaylandClient::QWaylandWindow *) window->handle();

    if (qWaylandWindow) {
        qDebug() << "MediaPlayer::iviSurfaceConfigure, configure QWaylandWindow size " << width << "," << height;
        qWaylandWindow->configure(0, width, height);
    }

    QQuickItem *object = rootObject();
    if (object) {
        qDebug() << "MediaPlayer::iviSurfaceConfigure, configure QQuickItem size " << width << "," << height;
       object->setWidth(width);
       object->setHeight(height);
   }
}

bool MediaPlayer::eventFilter(QObject *obj, QEvent *event) {
    //qDebug() << "MediaPlayer::eventFilter, event " << event;
    QWindow *window = qobject_cast<QWindow*>(obj);

    switch (event->type()) {
        case QEvent::Close:
        {
            qDebug() << "MediaPlayer has closed";
            window->destroy();
            break;
        }

        case QEvent::PlatformSurface:
        {
            QPlatformSurfaceEvent::SurfaceEventType eventType =
                static_cast<QPlatformSurfaceEvent *>(event)->surfaceEventType();

            if (eventType == QPlatformSurfaceEvent::SurfaceCreated) {
                qDebug() << "MediaPlayer has created surface";
                this->createSurface(window, QtWaylandClient::WRS_IVI_ID_SURFACE_DEFAULT);
            } else if (eventType == QPlatformSurfaceEvent::SurfaceAboutToBeDestroyed) {
                qDebug() << "MediaPlayer has destroyed surface";
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

    MediaPlayer media;
    media.show();

    return app.exec();
}