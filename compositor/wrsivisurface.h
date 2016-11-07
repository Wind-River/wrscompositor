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

#ifndef WRSIVISURFACE_H
#define WRSIVISURFACE_H

#include "wrscompositor.h"

/**
 * @brief The WrsIviSurface class
 *        Associate a wl_surface to an ivi_surface
 */
class WrsIviSurface
        : public QObject
        , public QtWaylandServer::ivi_surface
{
    Q_OBJECT
private:
    WrsCompositor *mCompositor;

public:
    WrsIviSurface(struct ::wl_display *display, WrsCompositor *compositor);
    WrsIviSurface(WrsCompositor *compositor, struct ::wl_client *client, int id, int version);

protected:
    void ivi_surface_bind_resource(QtWaylandServer::ivi_surface::Resource *resource);
    void ivi_surface_destroy_resource(QtWaylandServer::ivi_surface::Resource *resource);
    void ivi_surface_destroy(QtWaylandServer::ivi_surface::Resource *resource);

private:

public:
};

#endif // WRSIVISURFACE_H
