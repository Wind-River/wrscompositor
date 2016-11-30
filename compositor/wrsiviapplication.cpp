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
/*
 * Author: ionut.popa@windriver.com
 */
#include "wrsiviapplication.h"
#include "wrsivisurface.h"
#include "wrscompositor.h"
#include "wrslogging.h"

/**
 * @brief WrsIviApplication::WrsIviApplication - constructor for ivi_app interface
 * @param display
 * @param compositor
 */
WrsIviApplication::WrsIviApplication(struct ::wl_display *display, WrsCompositor *compositor)
        : QObject(compositor) {
    this->mCompositor = compositor;
    ivi_application::init(display, 1);
}

void WrsIviApplication::ivi_application_bind_resource(QtWaylandServer::ivi_application::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    DEBUG() << "resource " << resource;
    TRACE() << "[END]";
}

void WrsIviApplication::ivi_application_destroy_resource(QtWaylandServer::ivi_application::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}

void WrsIviApplication::ivi_application_surface_create(QtWaylandServer::ivi_application::Resource *resource, uint32_t ivi_id, struct ::wl_resource *surface, uint32_t id) {
    (void) resource;
    (void) ivi_id;
    (void) surface;

    TRACE() << "[BEGIN]";
    DEBUG() << "ivi_id:" << ivi_id << " id:" << id << "resource:" << resource;
    QWaylandSurface *qWlsurface = QWaylandSurface::fromResource(surface);

    //TODO: store the link beteen WrsIviSurface -> wl_surface in  GeniviWaylandIVIExtenssion

    WrsIviSurface *iviSurface = new WrsIviSurface(
                this->mCompositor,
                surface->client,
                id,
                1);

    WrsIVIModel::IVISurface *iviSurfaceModel = this->
            mCompositor->
            findIVISurfaceByQWaylandSurface(qWlsurface);

    iviSurfaceModel->addResourceForClient(surface->client, iviSurface->resource()->handle);
    iviSurfaceModel->setId(ivi_id);

    connect(qWlsurface, SIGNAL(sizeChanged()),
            this, SLOT(sizeChanged()));
    TRACE() << "[END]";
}


void WrsIviApplication::sizeChanged() {
    TRACE() << "[BEGIN]";
    QWaylandSurface *qWlsurface = qobject_cast<QWaylandSurface *>(sender());

    WrsIVIModel::IVISurface *iviSurfaceModel = this->
            mCompositor->
            findIVISurfaceByQWaylandSurface(qWlsurface);

    //Get IVI surface for QWaylandSurface
    QtWaylandServer::ivi_surface::Resource::fromResource(
                iviSurfaceModel->getResourceForClient(qWlsurface->client()->client())
        )->ivi_surface_object->send_configure(
                qWlsurface->size().width(), qWlsurface->size().height());

    DEBUG() << "Ivi app surface size changed: send_configure" << qWlsurface->size().width() << "," << qWlsurface->size().height();
    TRACE() << "[END]";
}
