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

#include "qiviwindow.h"
#include "qtwaylandiviapplication.h"

static void myRegistryListener(
        void *data,
        struct wl_registry *registry,
        uint32_t id,
        const QString &interface,
        uint32_t version) {

    qDebug() << "INTERFACE !!!" << interface;

    QIVIWindow *iviWindow = (QIVIWindow *) data;
    if (interface == "ivi_application") {
        qDebug() << "    - Registered to ivi_application";
        iviWindow->mIviApp->init(registry, id, version);
    }
    if (interface == "ivi_surface") {
        qDebug() << "    - Registered to ivi_surface";
        iviWindow->mIviSurface->init(registry, id, version);
    }
}


QIVIWindow::QIVIWindow(QtWaylandClient::QtWaylandIviApplication *iviApp, int iviId) : QMainWindow(NULL)
{
    this->mIviApp       = iviApp;
    this->mIviID        = iviId;
    this->mIviSurface   = new QtWaylandClient::QtWaylandIviSurface();
    this->mIviSurfaceInst = NULL;
}


void QIVIWindow::iviSurfaceConfigure(int w, int h) {
    printf("iviSurfaceConfigure>>>>>>>>>>>>>>>>>>!!>: %d %d, this->mIviID: %d\n", w, h, this->mIviID);
    //TODO: emit a signal
    if (this->mIviID == 4) {//PHONE
        if (w < 500 || h < 500) {
            printf(">>>>>>>>>>>>>> SMALL!!!\n");
            this->setStyleSheet("background-image: url(./phone_small.jpg); background-repeat:no-repeat; background-position: center; background-color: black;");

        } else {
            this->setStyleSheet("background-image: url(./phone.jpg); background-repeat:no-repeat; background-position: center; background-color: black;");
        }
    }
}


bool QIVIWindow::event(QEvent *event) {
    bool ret = QMainWindow::event(event);
    qDebug() << "Event:" << event;

    if (this->windowHandle() != NULL) {
        QtWaylandClient::QWaylandWindow  *w = (QtWaylandClient::QWaylandWindow  *) this->windowHandle()->handle();
        if (w != NULL) {
            if (w->screen() != 0) {
                if (w->isInitialized()) {
                    if (this->mIviApp->isInitialized()) {
                        if (this->mIviSurfaceInst == NULL) {
                            qDebug() << "    - Create IVI surface";
                            this->mIviSurfaceInst = new QtWaylandClient::QtWaylandIviSurface(
                                    this->mIviApp->surface_create(
                                            this->mIviID,
                                            w->object()));
                            this->mIviSurfaceInst->setParent(this);
                        }
                    } else {
                        if (w->display() != NULL) {
                            qDebug() << "    - Register to ivi_application interface";
                            w->display()->addRegistryListener(
                                    &myRegistryListener,
                                    this);
                        }
                    }
                }
            }
        }
    }
    return ret;
}
