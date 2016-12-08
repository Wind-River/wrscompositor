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

#include "qwaylandivisurface.h"
#include "phone.h"

namespace QtWaylandClient {

QWaylandIviSurface::QWaylandIviSurface(struct ::ivi_surface *ivi_surface)
    : QtWayland::ivi_surface(ivi_surface)
{
}

QWaylandIviSurface::QWaylandIviSurface(struct ::ivi_surface *ivi_surface,
                                       struct ::ivi_controller_surface *iviControllerSurface)
    : QtWayland::ivi_surface(ivi_surface)
    , QtWayland::ivi_controller_surface(iviControllerSurface)
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
    ((Phone *) parent())->iviSurfaceConfigure(width, height);
    //this->mWindow->configure(0, width, height);
}

}