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
#ifndef WRSIVIAPPLICATION_H
#define WRSIVIAPPLICATION_H

#include "wrscompositor.h"

class WrsIviApplication
        : public QObject
        , public QtWaylandServer::ivi_application
{
    Q_OBJECT
private:
    WrsCompositor *mCompositor;

public:
    WrsIviApplication(struct ::wl_display *display, WrsCompositor *compositor);

protected:
    virtual void ivi_application_bind_resource(Resource *resource);
    virtual void ivi_application_destroy_resource(Resource *resource);

    /**
     * @brief ivi_application_surface_create
     * @param resource
     * @param ivi_id
     * @param surface
     * @param id
     */
    virtual void ivi_application_surface_create(Resource *resource, uint32_t ivi_id, struct ::wl_resource *surface, uint32_t id);

private slots:
    /**
     * @brief sizeChanged - callback when QWaylandSurface size is updated
     *                      notify corresponding IVI surface that size was changed.
     */
    void sizeChanged();
};

#endif // WRSIVIAPPLICATION_H
