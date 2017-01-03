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

#ifndef QWAYLANDIVIEXTENSION_H
#define QWAYLANDIVIEXTENSION_H

#include <QDebug>
#include <QtGui/private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformnativeinterface.h>
#include <QtWaylandClient/private/qwaylandintegration_p.h>
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include "qwayland-ivi-application.h"
#include "qwayland-ivi-controller.h"

QT_BEGIN_NAMESPACE

namespace QtWaylandClient {

enum IVI_SURFACE_ROLE {
    WRS_IVI_ID_SURFACE_DEFAULT = 0x00,
    WRS_IVI_ID_SURFACE_NAVIGATION = 0x01,
    WRS_IVI_ID_SURFACE_DIALOG = 0x02,
    WRS_IVI_ID_SURFACE_PHONE = 0x03,
    WRS_IVI_ID_SURFACE_PROJECTION = 0x04,
    WRS_IVI_ID_SURFACE_CAMERA = 0x05
};

class QWaylandIviExtension;

class QWaylandIviSurface : public QtWayland::ivi_surface
                         , public QtWayland::ivi_controller_surface
{
public:
    QWaylandIviSurface(struct ::ivi_surface *ivi_surface);
    QWaylandIviSurface(struct ::ivi_surface *ivi_surface,
                       struct ::ivi_controller_surface *iviControllerSurface);
    virtual ~QWaylandIviSurface();
    void setParent(QWaylandIviExtension *extension) {
        mIviExtension = extension;
    }
private:
    virtual void ivi_surface_configure(int32_t width, int32_t height) Q_DECL_OVERRIDE;
    QWaylandIviExtension *mIviExtension;
};

class Q_WAYLAND_CLIENT_EXPORT QWaylandIviExtension
{
public:
    QWaylandIviExtension();
    ~QWaylandIviExtension();
    bool createSurface(QWindow *window, uint32_t role);
    void iviSurfaceConfigure(int width, int height);

protected:
    virtual void surfaceConfigure(int width, int height);

private:
    static void registryIvi(void *data, struct wl_registry *registry,
            uint32_t id, const QString &interface, uint32_t version);

private:
    QtWayland::ivi_application *mIviApplication;
    QtWayland::ivi_controller *mIviController;
    QWaylandIviSurface *mIviSurface;
};

}

QT_END_NAMESPACE

#endif // QWAYLANDIVIEXTENSION_H
