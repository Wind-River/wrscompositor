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
#ifndef WRSIVICONTROLLER_H
#define WRSIVICONTROLLER_H

#include "wrscompositor.h"
/**
 * @brief The WrsIviController class is implementing the ivi-controller.xml protocol
 *        It's connecting to WrsCompositor signals to get surface/layers/screen change events
 *
 */
class WrsIviController
        : public QtWaylandServer::ivi_controller_surface
        , public QtWaylandServer::ivi_controller_layer
        , public QtWaylandServer::ivi_controller_screen
        , public QtWaylandServer::ivi_controller

{
private:
    WrsCompositor *mCompositor;

public:
    WrsIviController(struct ::wl_display *display, WrsCompositor *compositor);

protected:
    void ivi_controller_surface_bind_resource(QtWaylandServer::ivi_controller_surface::Resource *resource);
    void ivi_controller_surface_destroy_resource(QtWaylandServer::ivi_controller_surface::Resource *resource);
    void ivi_controller_surface_set_visibility(QtWaylandServer::ivi_controller_surface::Resource *resource, uint32_t visibility);
    void ivi_controller_surface_set_opacity(QtWaylandServer::ivi_controller_surface::Resource *resource, wl_fixed_t opacity);
    void ivi_controller_surface_set_source_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);
    void ivi_controller_surface_set_destination_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);
    void ivi_controller_surface_set_configuration(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t width, int32_t height);
    void ivi_controller_surface_set_orientation(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t orientation);
    void ivi_controller_surface_screenshot(QtWaylandServer::ivi_controller_surface::Resource *resource, const QString &filename);
    void ivi_controller_surface_dosend_stats(QtWaylandServer::ivi_controller_surface::Resource *resource);
    void ivi_controller_surface_destroy(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t destroy_scene_object);

    void ivi_controller_layer_bind_resource(QtWaylandServer::ivi_controller_layer::Resource *resource);
    void ivi_controller_layer_destroy_resource(QtWaylandServer::ivi_controller_layer::Resource *resource);
    void ivi_controller_layer_set_visibility(QtWaylandServer::ivi_controller_layer::Resource *resource, uint32_t visibility);
    void ivi_controller_layer_set_opacity(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_fixed_t opacity);
    void ivi_controller_layer_set_source_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);
    void ivi_controller_layer_set_destination_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height);
    void ivi_controller_layer_set_configuration(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t width, int32_t height);
    void ivi_controller_layer_set_orientation(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t orientation);
    void ivi_controller_layer_screenshot(QtWaylandServer::ivi_controller_layer::Resource *resource, const QString &filename);
    void ivi_controller_layer_clear_surfaces(QtWaylandServer::ivi_controller_layer::Resource *resource);
    void ivi_controller_layer_add_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface);
    void ivi_controller_layer_remove_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface);
    void ivi_controller_layer_set_render_order(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_array *id_surfaces);
    void ivi_controller_layer_destroy(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t destroy_scene_object);

    void ivi_controller_screen_bind_resource(QtWaylandServer::ivi_controller_screen::Resource *resource);
    void ivi_controller_screen_destroy_resource(QtWaylandServer::ivi_controller_screen::Resource *resource);
    void ivi_controller_screen_destroy(QtWaylandServer::ivi_controller_screen::Resource *resource);
    void ivi_controller_screen_clear(QtWaylandServer::ivi_controller_screen::Resource *resource);
    void ivi_controller_screen_add_layer(QtWaylandServer::ivi_controller_screen::Resource *resource, struct ::wl_resource *layer);
    void ivi_controller_screen_screenshot(QtWaylandServer::ivi_controller_screen::Resource *resource, const QString &filename);
    void ivi_controller_screen_set_render_order(QtWaylandServer::ivi_controller_screen::Resource *resource, wl_array *id_layers);

    void ivi_controller_bind_resource(QtWaylandServer::ivi_controller::Resource *resource);
    void ivi_controller_destroy_resource(QtWaylandServer::ivi_controller::Resource *resource);
    void ivi_controller_commit_changes(QtWaylandServer::ivi_controller::Resource *resource);
    void ivi_controller_layer_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_layer, int32_t width, int32_t height, uint32_t id);
    void ivi_controller_surface_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_surface, uint32_t id);

};

#endif // WRSIVICONTROLLER_H
