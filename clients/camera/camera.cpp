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

#include "camera.h"

Camera::Camera(QWidget * parent)
    :  QQuickWidget(parent) {
    setSource(QUrl("qrc:///main.qml"));
}

Camera::~Camera() {
}

void Camera::surfaceConfigure(int width, int height) {
    qDebug() << __func__ << __LINE__;

    QtWaylandClient::QWaylandWindow *window = 
                            (QtWaylandClient::QWaylandWindow *) this->windowHandle()->handle();

    if (window) {
        qDebug() << "Camera::iviSurfaceConfigure, configure QWaylandWindow size " << width << "," << height;
        window->configure(0, width, height);
    }

    QQuickItem *object = rootObject();
    if (object) {
        qDebug() << "Camera::iviSurfaceConfigure, configure QQuickItem size " << width << "," << height;
       object->setWidth(width);
       object->setHeight(height);
   }
}

bool Camera::event(QEvent *event) {
    bool ret = QWidget::event(event);
    //qDebug() << "Camera::event, event's type = " << event->type();

    if (event->type() == QEvent::PlatformSurface) {
        QWindow *window = this->windowHandle();
        ret = this->createSurface(window, QtWaylandClient::WRS_IVI_ID_SURFACE_CAMERA);
    }

    return ret;
}
