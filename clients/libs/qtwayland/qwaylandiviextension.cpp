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

#include "qwaylandiviextension.h"

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

QWaylandIviSurface::QWaylandIviSurface(struct ::ivi_surface *ivi_surface)
    : QtWayland::ivi_surface(ivi_surface)
    , mIviExtension(Q_NULLPTR)
{
}

QWaylandIviSurface::QWaylandIviSurface(struct ::ivi_surface *ivi_surface,
                                       struct ::ivi_controller_surface *iviControllerSurface)
    : QtWayland::ivi_surface(ivi_surface)
    , QtWayland::ivi_controller_surface(iviControllerSurface)
    , mIviExtension(Q_NULLPTR)
{
}

QWaylandIviSurface::~QWaylandIviSurface()
{
    ivi_surface::destroy();
    if (QtWayland::ivi_controller_surface::object())
        QtWayland::ivi_controller_surface::destroy(0);
}

void QWaylandIviSurface::ivi_surface_configure(int32_t width, int32_t height)
{
    qDebug() << " QWaylandIviSurface::ivi_surface_configure, width = " << width << " height = " << height;
    if (mIviExtension != NULL)
        mIviExtension->iviSurfaceConfigure(width, height);
}

QWaylandIviExtension::QWaylandIviExtension()
    : mIviApplication(Q_NULLPTR)
    , mIviController(Q_NULLPTR)
    , mIviSurface(Q_NULLPTR)
    , mWindow(Q_NULLPTR)
{
    if (!QGuiApplication::platformNativeInterface() || !QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("wl_display")) {
        qDebug() << "This application requires a wayland plugin";
        return;
    }

    QtWaylandClient::QWaylandIntegration *wayland_integration = static_cast<QtWaylandClient::QWaylandIntegration *>(QGuiApplicationPrivate::platformIntegration());

    QtWaylandClient::QWaylandDisplay *wayland_display = wayland_integration->display();
    wayland_display->addRegistryListener(registryIvi, this);
}

QWaylandIviExtension::~QWaylandIviExtension()
{
    delete mIviApplication;
    delete mIviController;
    delete mIviSurface;
}

void QWaylandIviExtension::iviSurfaceConfigure(int width, int height) {
    this->surfaceConfigure(mWindow, width, height);
}

void QWaylandIviExtension::surfaceConfigure(QWindow* window, int width, int height) {

}

void QWaylandIviExtension::registryIvi(void *data,
                                        struct wl_registry *registry,
                                        uint32_t id,
                                        const QString &interface,
                                        uint32_t version)
{
    QWaylandIviExtension *iviExtension = static_cast<QWaylandIviExtension *>(data);

    qDebug() << "QWaylandIviExtenstion::registryIvi, interface = " << interface;

    if (interface == QStringLiteral("ivi_application"))
        iviExtension->mIviApplication = new QtWayland::ivi_application(registry, id, version);

#if 0
    if (interface == QStringLiteral("ivi_controller"))
        iviExtension->mIviController = new QtWayland::ivi_controller(registry, id, version);
#endif
}

bool QWaylandIviExtension::createSurface(QWindow* window, uint32_t role) {
    bool result = false;
    QtWaylandClient::QWaylandWindow *qwayland_window = (QtWaylandClient::QWaylandWindow *) window->handle();

    if (!qwayland_window)
        return result;

    if (qwayland_window->screen() != 0) {
        if (qwayland_window->isInitialized()) {
            struct ivi_surface *surface = mIviApplication->surface_create(role, qwayland_window->object());
#if 0
            struct ::ivi_controller_surface *controller = mIviController->ivi_controller::surface_create(mSurfaceId);
#endif
            mWindow = window;
            mIviSurface = new QWaylandIviSurface(surface);
            mIviSurface->setParent(this);
            result = true;
        }
    }

    return result;
}

}

QT_END_NAMESPACE
