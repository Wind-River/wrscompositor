#include "wrsivicontroller.h"
#include "wrslogging.h"

WrsIviController::WrsIviController(struct ::wl_display *display, WrsCompositor *compositor)
{
    this->mCompositor = compositor;
    ivi_controller::init(display, 1);
    ivi_controller_screen::init(display, 1);
    ivi_controller_layer::init(display, 1);
    ivi_controller_surface::init(display, 1);
}


void WrsIviController::ivi_controller_surface_bind_resource(QtWaylandServer::ivi_controller_surface::Resource *resource) {
    (void) resource;

    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_destroy_resource(QtWaylandServer::ivi_controller_surface::Resource *resource) {
    (void) resource;

    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_set_visibility(QtWaylandServer::ivi_controller_surface::Resource *resource, uint32_t visibility) {
    (void) resource;

    TRACE() << "[BEGIN]";
    WrsIVIModel::IVISurface *surface = mCompositor->
            getIviScene()->
            findSurfaceByResource(resource->handle);
    if(!surface) {
        TRACE() << "[END]";
        return;
    }
    DEBUG() << "surface id " << surface->id() << visibility;
    surface->setVisibility(visibility);
    surface->qmlWindowFrame()->setProperty("visible", visibility);
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_set_opacity(QtWaylandServer::ivi_controller_surface::Resource *resource, wl_fixed_t opacity) {
    (void) resource;

    TRACE() << "[BEGIN]";
    WrsIVIModel::IVISurface *surface = mCompositor->
            getIviScene()->
            findSurfaceByResource(resource->handle);
    if(!surface) {
        TRACE() << "[END]";
        return;
    }
    DEBUG() << "surface id " << surface->id() << opacity;
    surface->setOpacity(wl_fixed_to_double(opacity));
    surface->qmlWindowFrame()->setProperty("opacity", wl_fixed_to_double(opacity));
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_set_source_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_set_destination_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void) resource;
    TRACE() << "[BEGIN";
    WrsIVIModel::IVISurface *surface = mCompositor->
            getIviScene()->
            findSurfaceByResource(resource->handle);
    if(!surface) {
        TRACE() << "[END]";
        return;
    }
    DEBUG() << "Surface: id " << surface->id() << " [x, y, w, h]=[" << x << ", " << y << ", " << width << ", " << height << "]";
    surface->qmlWindowFrame()->setProperty("x", x);
    surface->qmlWindowFrame()->setProperty("y", y);
    double originalWidth = surface->qmlWindowFrame()->property("targetWidth").toDouble();
    double originalHeight = surface->qmlWindowFrame()->property("targetHeight").toDouble();
    surface->qmlWindowFrame()->setProperty("scaledWidth", width/originalWidth);
    surface->qmlWindowFrame()->setProperty("scaledHeight", height/originalHeight);
    surface->setX(x);
    surface->setY(y);
    surface->setWidth(width);
    surface->setHeight(height);
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_set_configuration(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t width, int32_t height) {
    (void) resource;
    TRACE() << "[BEGIN]";
    WrsIVIModel::IVISurface *surface = mCompositor->
            getIviScene()->
            findSurfaceByResource(resource->handle);
    if (!surface) {
        TRACE() << "[END]";
        return;
    }
    DEBUG() << "Width:" << width << " Height:" << height;
    surface->qmlWindowFrame()->setProperty("width", width);
    surface->qmlWindowFrame()->setProperty("height", height);
    surface->setWidth(width);
    surface->setHeight(height);
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_set_orientation(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t orientation) {
    (void) resource;
    TRACE() << "[BEGIN]";
    WrsIVIModel::IVISurface *surface = mCompositor->
            getIviScene()->
            findSurfaceByResource(resource->handle);
    if (!surface) {
        TRACE() << "[END]";
        return;
    }
    switch (orientation) {
        case 0:
            surface->qmlWindowFrame()->setProperty("rotation", 0);
            break;
        case 1:
            surface->qmlWindowFrame()->setProperty("rotation", 90);
            break;
        case 2:
            surface->qmlWindowFrame()->setProperty("rotation", 180);
            break;
        case 3:
            surface->qmlWindowFrame()->setProperty("rotation", 270);
            break;
        default:
            break;
    }
    if(orientation>=0 && orientation<4) {
        surface->setOrientation(orientation);
    }
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_screenshot(QtWaylandServer::ivi_controller_surface::Resource *resource, const QString &filename) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_dosend_stats(QtWaylandServer::ivi_controller_surface::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_destroy(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t destroy_scene_object) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_bind_resource(QtWaylandServer::ivi_controller_layer::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_destroy_resource(QtWaylandServer::ivi_controller_layer::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_set_visibility(QtWaylandServer::ivi_controller_layer::Resource *resource, uint32_t visibility) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_set_opacity(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_fixed_t opacity) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_set_source_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_set_destination_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_set_configuration(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t width, int32_t height) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_set_orientation(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t orientation) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_screenshot(QtWaylandServer::ivi_controller_layer::Resource *resource, const QString &filename) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_clear_surfaces(QtWaylandServer::ivi_controller_layer::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_add_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_remove_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_set_render_order(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_array *id_surfaces) {
    (void) resource;
    (void)id_surfaces;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_destroy(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t destroy_scene_object) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_screen_bind_resource(QtWaylandServer::ivi_controller_screen::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_screen_destroy_resource(QtWaylandServer::ivi_controller_screen::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_screen_destroy(QtWaylandServer::ivi_controller_screen::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_screen_clear(QtWaylandServer::ivi_controller_screen::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_screen_add_layer(QtWaylandServer::ivi_controller_screen::Resource *resource, struct ::wl_resource *layer) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_screen_screenshot(QtWaylandServer::ivi_controller_screen::Resource *resource, const QString &filename) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_screen_set_render_order(QtWaylandServer::ivi_controller_screen::Resource *resource, wl_array *id_layers) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_bind_resource(QtWaylandServer::ivi_controller::Resource *resource) {
    TRACE() << "[BEGIN]";
    for(int i=0; i<mCompositor->getIviScene()->screenCount(); i++) {
        WrsIVIModel::IVIScreen *screen = mCompositor->getIviScene()->screen(i);
        QtWayland::Output *output = screen->waylandOutput()->handle();
        // find wl_output for client
        if(output->resourceMap().contains(resource->client())) {
            // get wl_resource of wl_output for client
            QtWaylandServer::wl_output::Resource *output_resource = output->resourceMap().value(resource->client());

            // init screen interface for client
            QtWaylandServer::ivi_controller_screen::init(resource->handle->client, 0, 1);
            // send screen(client, r-id of output for screen, screen)
            QtWaylandServer::ivi_controller::send_screen(resource->handle, wl_resource_get_id(output_resource->handle), QtWaylandServer::ivi_controller_screen::resource()->handle);
        }
    }

    // XXX should reverse ???
    for(int i=0; i<mCompositor->getIviScene()->screenCount(); i++) {
        WrsIVIModel::IVIScreen *screen = mCompositor->getIviScene()->screen(i);
        for(int j=0; j<screen->layerCount(); j++) {
            WrsIVIModel::IVILayer *layer = screen->layer(j);
            DEBUG() << "send layer id" << layer->id();

            // send layer
            QtWaylandServer::ivi_controller::send_layer(resource->handle, layer->id());
            for(int k=0; k<layer->surfaceCount(); k++) {
                WrsIVIModel::IVISurface *surface = layer->surface(k);
                DEBUG() << "send surface id" << surface->id();

                // send layer
                QtWaylandServer::ivi_controller::send_surface(resource->handle, surface->id());
            }
        }
    }
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_destroy_resource(QtWaylandServer::ivi_controller::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_commit_changes(QtWaylandServer::ivi_controller::Resource *resource) {
    (void) resource;
    TRACE() << "[BEGIN]";
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_layer_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_layer, int32_t width, int32_t height, uint32_t id) {
    TRACE() << "[BEGIN]";
    QtWaylandServer::ivi_controller_layer::init(resource->handle->client, id, 1);
    struct wl_resource *resource_ctrllayer = QtWaylandServer::ivi_controller_layer::resource()->handle;
    for(int i=0; i<mCompositor->getIviScene()->mainScreen()->layerCount(); i++) {
        WrsIVIModel::IVILayer *layer = mCompositor->getIviScene()->mainScreen()->layer(i);
        if(layer->id() == (int)id_layer) {
            layer->setWaylandResource(resource_ctrllayer);
            DEBUG() << "send layer" << id_layer;
            ivi_controller_layer::send_opacity(resource_ctrllayer, wl_fixed_from_double(layer->opacity()));
            ivi_controller_layer::send_source_rectangle(resource_ctrllayer, layer->x(), layer->y(), layer->width(), layer->height());
            ivi_controller_layer::send_destination_rectangle(resource_ctrllayer, layer->x(), layer->y(), layer->width(), layer->height());
            ivi_controller_layer::send_orientation(resource_ctrllayer, layer->orientation());
            ivi_controller_layer::send_visibility(resource_ctrllayer, layer->visibility());
            // XXX REMOVE layer from specific screen why ??
            // send_layer_event of ivi-controller-impl.c
            QtWaylandServer::ivi_controller_layer::send_screen(resource_ctrllayer, NULL);

            QtWayland::Output *output = layer->screen()->waylandOutput()->handle();
            // get wl_resource of wl_output for client
            QtWaylandServer::wl_output::Resource *output_resource = output->resourceMap().value(resource->client());

            // send parent screen
            QtWaylandServer::ivi_controller_layer::send_screen(resource_ctrllayer, output_resource->handle);
            break;
        }
    }
    TRACE() << "[END]";
}


void WrsIviController::ivi_controller_surface_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_surface, uint32_t id) {
    (void) resource;
    TRACE() << "[BEGIN]";
    WrsIVIModel::IVISurface *surface = NULL;
    WrsIVIModel::IVILayer *parentLayer = NULL;
    for(int i=0; i<mCompositor->getIviScene()->screenCount(); i++) {
        WrsIVIModel::IVIScreen *screen = mCompositor->getIviScene()->screen(i);
        for(int j=0; j<screen->layerCount(); j++) {
            WrsIVIModel::IVILayer *layer = screen->layer(j);
            for(int k=0; k<layer->surfaceCount(); k++) {
                WrsIVIModel::IVISurface *_surface = layer->surface(k);
                if(_surface->id() == (int)id_surface) {
                    surface = _surface;
                    parentLayer = layer;
                    DEBUG() << "found surface " << id << surface;
                    break;
                }
            }
        }
    }
    QtWaylandServer::ivi_controller_surface::init(resource->handle->client, id, 1);
    struct wl_resource *resource_ctrlsurface = QtWaylandServer::ivi_controller_surface::resource()->handle;
    surface->setWaylandResource(resource_ctrlsurface);

    ivi_controller_surface::send_opacity(resource_ctrlsurface, wl_fixed_from_double(surface->opacity()));
    ivi_controller_surface::send_source_rectangle(resource_ctrlsurface, surface->x(), surface->y(), surface->width(), surface->height());
    ivi_controller_surface::send_destination_rectangle(resource_ctrlsurface, surface->x(), surface->y(), surface->width(), surface->height());
    ivi_controller_surface::send_orientation(resource_ctrlsurface, surface->orientation());
    ivi_controller_surface::send_visibility(resource_ctrlsurface, surface->visibility());
    // XXX REMOVE surface from specific screen why ??
    QtWaylandServer::ivi_controller_surface::send_layer(resource_ctrlsurface, NULL);
    // send parent layer
    QtWaylandServer::ivi_controller_surface::send_layer(resource_ctrlsurface, parentLayer->waylandResource());

    ivi_controller_surface::send_configuration(resource_ctrlsurface, surface->width(), surface->height());
    TRACE() << "[END]";
}
