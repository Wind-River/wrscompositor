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

#include "qwayland-common.h"

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

QWaylandIviSurface::QWaylandIviSurface(struct ::ivi_surface *ivi_surface)
    : QtWayland::ivi_surface(ivi_surface)
{
    mWaylandCommon = NULL;
}

QWaylandIviSurface::QWaylandIviSurface(struct ::ivi_surface *ivi_surface,
                                       struct ::ivi_controller_surface *iviControllerSurface)
    : QtWayland::ivi_surface(ivi_surface)
    , QtWayland::ivi_controller_surface(iviControllerSurface)
{
    mWaylandCommon = NULL;
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
    if (mWaylandCommon != NULL)
        mWaylandCommon->ivi_surface_configure(width, height);
}

QWaylandCommon::QWaylandCommon(uint32_t role)
    : mWindow(Q_NULLPTR)
    , mIviApplication(Q_NULLPTR)
    , mIviController(Q_NULLPTR)
    , mIviSurface(Q_NULLPTR)
    , mWaylandInputPanel(Q_NULLPTR)
    , mWaylandInputMethod(Q_NULLPTR)
    , mRole(role)
{
    if (!QGuiApplication::platformNativeInterface() ||
        !QGuiApplication::platformNativeInterface()->nativeResourceForIntegration("wl_display")) {
        qDebug() << "This application requires a wayland plugin";
        return;
    }

    QtWaylandClient::QWaylandIntegration *wayland_integration =
                static_cast<QtWaylandClient::QWaylandIntegration *>(QGuiApplicationPrivate::platformIntegration());

    mWaylandDisplay = wayland_integration->display();
    mWaylandDisplay->addRegistryListener(registryIvi, this);
}

QWaylandCommon::~QWaylandCommon()
{
    delete mWindow;
    delete mIviApplication;
    delete mIviController;
    delete mWaylandInputPanel;
    delete mWaylandInputMethod;
    delete mIviSurface;
}

 void QWaylandCommon::ivi_surface_configure(int width, int height) {
    this->configureIviSurface(mWindow, width, height);
 }

 void QWaylandCommon::configureIviSurface(QWindow *window, int width, int height)
 {
    qDebug() << "QWaylandCommon::configureIviSurface, configure QWaylandWindow size " << width << "," << height;

    QtWaylandClient::QWaylandWindow *qWaylandWindow =
                                    (QtWaylandClient::QWaylandWindow *) window->handle();
    if (qWaylandWindow)
        qWaylandWindow->configure(0, width, height);
 }

void QWaylandCommon::registryIvi(void *data,
                                struct wl_registry *registry,
                                uint32_t id,
                                const QString &interface,
                                uint32_t version)
{
    QWaylandCommon *common= static_cast<QWaylandCommon *>(data);

    qDebug() << "QWaylandCommon::registryIvi, interface = " << interface;

    if (interface == QStringLiteral("ivi_application")) {
        common->mIviApplication =
                    new QtWayland::ivi_application(registry, id, version);
    } else if (interface == QStringLiteral("ivi_controller")) {
        common->mIviController =
                    new QtWayland::ivi_controller(registry, id, version);
    } else {
        if (common->mRole == WRS_IVI_ID_SURFACE_KEYBOARD) {
            if (interface == QStringLiteral("wl_input_panel")) {
                common->mWaylandInputPanel =
                        new QtWayland::wl_input_panel(registry, id, version);
            } else if (interface == QStringLiteral("wl_input_method")) {
                common->mWaylandInputMethod =
                        new QtWaylandClient::QWaylandInputMethod(registry, id, version);
            }
        }
    }

    common->mWaylandDisplay->forceRoundTrip();
}

bool QWaylandCommon::createIviSurface(QWindow* window) {
    bool result = false;
    QtWaylandClient::QWaylandWindow *qwayland_window = (QtWaylandClient::QWaylandWindow *) window->handle();
    if (!qwayland_window || !mIviApplication)
        return result;

    if (qwayland_window->screen() != 0) {
        if (qwayland_window->isInitialized()) {
            struct ivi_surface *surface = mIviApplication->surface_create(mRole, qwayland_window->object());
            //struct ::ivi_controller_surface *controller = mIviApplication->ivi_controller::surface_create(mSurfaceId);
            mWindow = window;
            mIviSurface = new QtWaylandClient::QWaylandIviSurface(surface);
            mIviSurface->setQWaylandCommon(this);
            result = true;
        }
    }

    return result;
}

struct wl_display* QWaylandCommon::getWaylandDisplay()
{
    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    return static_cast<struct wl_display *>(native->nativeResourceForIntegration("wl_display"));
}

struct wl_output* QWaylandCommon::getWaylandOutput()
{
    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    return static_cast<struct wl_output*>(native->nativeResourceForScreen("output", QGuiApplication::primaryScreen()));
}

struct wl_surface* QWaylandCommon::getWaylandSurface(QWindow *window) {
    QPlatformNativeInterface *native = QGuiApplication::platformNativeInterface();
    return static_cast<struct wl_surface *>(native->nativeResourceForWindow("surface", window));
}

}

QT_END_NAMESPACE