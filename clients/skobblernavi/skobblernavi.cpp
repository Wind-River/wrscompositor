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

#include "skobblernavi.h"

SkobblerNavi::SkobblerNavi(QWidget* parent)
    : QWebView(parent), mSurfaceId(WRS_IVI_ID_SURFACE_NAVIGATION) {

    QUrl url = QUrl("qrc:///html5/index.html");
    load(url);


    if (!QGuiApplication::platformNativeInterface() || !QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("wl_display")) {
    	qDebug() << "This application requires a wayland plugin";
    	QCoreApplication::quit();
    	return;
    }

    QtWaylandClient::QWaylandIntegration *wayland_integration = static_cast<QtWaylandClient::QWaylandIntegration *>(QGuiApplicationPrivate::platformIntegration());

    QtWaylandClient::QWaylandDisplay *wayland_display = wayland_integration->display();
    wayland_display->addRegistryListener(registryIvi, this);
}

SkobblerNavi::~SkobblerNavi() {
    delete mIviApplication;
    delete mIviController;
    delete mIviSurface;
}

void SkobblerNavi::registryIvi(void *data,
                               struct wl_registry *registry,
                               uint32_t id,
                               const QString &interface,
                               uint32_t version)
{
    SkobblerNavi *navi = static_cast<SkobblerNavi *>(data);

    //qDebug() << "SkobblerNavi::registryIvi, interface = " << interface;

    if (interface == QStringLiteral("ivi_application"))
        navi->mIviApplication = new QtWayland::ivi_application(registry, id, version);

    // if (interface == QStringLiteral("ivi_controller"))
    //  navi->mIviController = new QtWayland::ivi_controller(registry, id, version);
}

bool SkobblerNavi::createSurface() {
    if (!this->windowHandle())
        return false;

    QtWaylandClient::QWaylandWindow *window = (QtWaylandClient::QWaylandWindow *) this->windowHandle()->handle();

    if (!window)
        return false;

    if (window->screen() != 0) {
        if (window->isInitialized()) {
            struct ivi_surface *surface = mIviApplication->surface_create(mSurfaceId, window->object());
            // struct ::ivi_controller_surface *controller = mIviController->ivi_controller::surface_create(mSurfaceId);

            mIviSurface = new QtWaylandClient::QWaylandIviSurface(surface, window);
        }
    }

    return true;
}

bool SkobblerNavi::event(QEvent *event) {
    bool ret = QWebView::event(event);
    //qDebug() << "SkobblerNavi::event, event's type = " << event->type();

    if (event->type() == QEvent::PlatformSurface) {
        ret = this->createSurface();
    }

    return ret;
}
