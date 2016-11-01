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

#include "wrsivisurface.h"
#include "wrscompositor.h"
#include "wrslogging.h"


/**
 * @brief WrsIviSurface::WrsIviSurface - constructor for ivi_surface interface
 * @param display
 * @param compositor
 */
WrsIviSurface::WrsIviSurface(struct ::wl_display *display, WrsCompositor *compositor)
        : QObject(compositor) {
    this->mCompositor = compositor;
    ivi_surface::init(display, 1);
}


/**
 * @brief WrsIviSurface::WrsIviSurface - constructor for and ivi_surface instance
 * @param compositor
 * @param client
 * @param id
 * @param version
 */
WrsIviSurface::WrsIviSurface(WrsCompositor *compositor, struct ::wl_client *client, int id, int version) {
    this->mCompositor = compositor;
    ivi_surface::init(client, id, version);
}


void WrsIviSurface::ivi_surface_bind_resource(QtWaylandServer::ivi_surface::Resource *resource) {
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviSurface::ivi_surface_destroy_resource(QtWaylandServer::ivi_surface::Resource *resource) {
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviSurface::ivi_surface_destroy(QtWaylandServer::ivi_surface::Resource *resource) {
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}
