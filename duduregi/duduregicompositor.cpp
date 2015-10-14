#include "duduregiconfig.h"
#include "duduregicompositor.h"

DuduregiCompositor::DuduregiCompositor()
#if DUDUREGI_WAYLAND_COMPOSITOR
        : QWaylandQuickCompositor(0, DefaultExtensions | SubSurfaceExtension)
        //, QtWaylandServer::ivi_controller_surface(QWaylandCompositor::handle()->wl_display(), 1)
        //, QtWaylandServer::ivi_controller_layer(QWaylandCompositor::handle()->wl_display(), 1)
        //, QtWaylandServer::ivi_controller_screen(QWaylandCompositor::handle()->wl_display(), 1)
              , m_fullscreenSurface(0), mGeniviExt(0)

#endif
{
    setTitle(QLatin1String("Wind River Duduregi Wayland Compositor"));
    setSource(QUrl("main.qml"));
    setResizeMode(QQuickView::SizeRootObjectToView);
    setColor(Qt::black);
    winId();

    rootContext()->setContextProperty("compositor", this);

#if DUDUREGI_WAYLAND_COMPOSITOR
    addDefaultShell();
    createOutput(this, DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME);
    setClientFullScreenHint(true);
    connect(this, SIGNAL(afterRendering()), this, SLOT(sendCallbacks()));

    QtWaylandServer::ivi_controller::init(QWaylandCompositor::handle()->wl_display(), 1);
#endif


#if DUDUREGI_WAYLAND_COMPOSITOR
    QObject::connect(this, SIGNAL(windowAdded(QVariant)), rootObject(), SLOT(windowAdded(QVariant)));
    QObject::connect(this, SIGNAL(windowResized(QVariant)), rootObject(), SLOT(windowResized(QVariant)));
#endif

}

DuduregiCompositor::~DuduregiCompositor() {
    QSettings settings(DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME);
    //settings.setValue("size", size());
    //settings.setValue("windowState", windowState());
    settings.setValue("position", position());
}

#if DUDUREGI_WAYLAND_COMPOSITOR
QWaylandQuickSurface* DuduregiCompositor::fullscreenSurface() const
{
    return m_fullscreenSurface;
}

Q_INVOKABLE QWaylandSurfaceItem* DuduregiCompositor::item(QWaylandSurface *surf)
{
    return static_cast<QWaylandSurfaceItem *>(surf->views().first());
}


void DuduregiCompositor::destroyWindow(QVariant window) {
    qvariant_cast<QObject *>(window)->deleteLater();
}

void DuduregiCompositor::setFullscreenSurface(QWaylandQuickSurface *surface) {
    if (surface == m_fullscreenSurface)
        return;
    m_fullscreenSurface = surface;
    emit fullscreenSurfaceChanged();
}

void DuduregiCompositor::surfaceMapped() {
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());
    emit windowAdded(QVariant::fromValue(surface));
}
void DuduregiCompositor::surfaceUnmapped() {
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());
    if (surface == m_fullscreenSurface)
        m_fullscreenSurface = 0;
}

void DuduregiCompositor::surfaceDestroyed() {
    QWaylandQuickSurface *surface = static_cast<QWaylandQuickSurface *>(sender());
    if (surface == m_fullscreenSurface)
        m_fullscreenSurface = 0;
}

void DuduregiCompositor::sendCallbacks() {
    if (m_fullscreenSurface)
        sendFrameCallbacks(QList<QWaylandSurface *>() << m_fullscreenSurface);
    else
        sendFrameCallbacks(surfaces());
}

void DuduregiCompositor::resizeEvent(QResizeEvent *event)
{
    QQuickView::resizeEvent(event);
    QWaylandCompositor::setOutputGeometry(QRect(0, 0, width(), height()));

    if(!mGeniviExt) {
        mGeniviExt = new GeniviWaylandIVIExtension::IVIScene(this, width(), height(), this);
        rootContext()->setContextProperty("geniviExt", mGeniviExt);
    }
}

void DuduregiCompositor::surfaceCreated(QWaylandSurface *surface) {
    connect(surface, SIGNAL(surfaceDestroyed()), this, SLOT(surfaceDestroyed()));
    connect(surface, SIGNAL(mapped()), this, SLOT(surfaceMapped()));
    connect(surface,SIGNAL(unmapped()), this,SLOT(surfaceUnmapped()));
}


void DuduregiCompositor::ivi_controller_surface_bind_resource(QtWaylandServer::ivi_controller_surface::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_surface_destroy_resource(QtWaylandServer::ivi_controller_surface::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_surface_set_visibility(QtWaylandServer::ivi_controller_surface::Resource *resource, uint32_t visibility) {
    (void)resource;
    qDebug() << __func__ << visibility;
};
void DuduregiCompositor::ivi_controller_surface_set_opacity(QtWaylandServer::ivi_controller_surface::Resource *resource, wl_fixed_t opacity) {
    (void)resource;
    qDebug() << __func__ << opacity;
};
void DuduregiCompositor::ivi_controller_surface_set_source_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << x << y << width << height;
};
void DuduregiCompositor::ivi_controller_surface_set_destination_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << x << y << width << height;
};
void DuduregiCompositor::ivi_controller_surface_set_configuration(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << width << height;
};
void DuduregiCompositor::ivi_controller_surface_set_orientation(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t orientation) {
    (void)resource;
    qDebug() << __func__ << orientation;
};
void DuduregiCompositor::ivi_controller_surface_screenshot(QtWaylandServer::ivi_controller_surface::Resource *resource, const QString &filename) {
    (void)resource;
    qDebug() << __func__ << filename;
};
void DuduregiCompositor::ivi_controller_surface_dosend_stats(QtWaylandServer::ivi_controller_surface::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_surface_destroy(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t destroy_scene_object) {
    (void)resource;
    qDebug() << __func__ << destroy_scene_object;
};



void DuduregiCompositor::ivi_controller_layer_bind_resource(QtWaylandServer::ivi_controller_layer::Resource *resource) {
    (void)resource;
    qDebug() << __func__ ;
};
void DuduregiCompositor::ivi_controller_layer_destroy_resource(QtWaylandServer::ivi_controller_layer::Resource *resource) {
    (void)resource;
    qDebug() << __func__ ;
};
void DuduregiCompositor::ivi_controller_layer_set_visibility(QtWaylandServer::ivi_controller_layer::Resource *resource, uint32_t visibility) {
    (void)resource;
    qDebug() << __func__ << visibility;
};
void DuduregiCompositor::ivi_controller_layer_set_opacity(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_fixed_t opacity) {
    (void)resource;
    qDebug() << __func__ << opacity;
};
void DuduregiCompositor::ivi_controller_layer_set_source_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << x << y << width << height;
};
void DuduregiCompositor::ivi_controller_layer_set_destination_rectangle(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << x << y << width << height;
};
void DuduregiCompositor::ivi_controller_layer_set_configuration(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << width << height;
};
void DuduregiCompositor::ivi_controller_layer_set_orientation(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t orientation) {
    (void)resource;
    qDebug() << __func__ << orientation;
};
void DuduregiCompositor::ivi_controller_layer_screenshot(QtWaylandServer::ivi_controller_layer::Resource *resource, const QString &filename) {
    (void)resource;
    qDebug() << __func__ << filename;
};
void DuduregiCompositor::ivi_controller_layer_clear_surfaces(QtWaylandServer::ivi_controller_layer::Resource *resource) {
    (void)resource;
};
void DuduregiCompositor::ivi_controller_layer_add_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface) {
    (void)resource;
    qDebug() << __func__ << surface;
};
void DuduregiCompositor::ivi_controller_layer_remove_surface(QtWaylandServer::ivi_controller_layer::Resource *resource, struct ::wl_resource *surface) {
    (void)resource;
    qDebug() << __func__ << surface;
};
void DuduregiCompositor::ivi_controller_layer_set_render_order(QtWaylandServer::ivi_controller_layer::Resource *resource, wl_array *id_surfaces) {
    (void)resource;
    (void)id_surfaces;
};
void DuduregiCompositor::ivi_controller_layer_destroy(QtWaylandServer::ivi_controller_layer::Resource *resource, int32_t destroy_scene_object) {
    (void)resource;
    qDebug() << __func__ << destroy_scene_object;
};



void DuduregiCompositor::ivi_controller_screen_bind_resource(QtWaylandServer::ivi_controller_screen::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_screen_destroy_resource(QtWaylandServer::ivi_controller_screen::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_screen_destroy(QtWaylandServer::ivi_controller_screen::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_screen_clear(QtWaylandServer::ivi_controller_screen::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_screen_add_layer(QtWaylandServer::ivi_controller_screen::Resource *resource, struct ::wl_resource *layer) {
    (void)resource;
    qDebug() << __func__ << layer;
};
void DuduregiCompositor::ivi_controller_screen_screenshot(QtWaylandServer::ivi_controller_screen::Resource *resource, const QString &filename) {
    (void)resource;
    qDebug() << __func__ << filename;
};
void DuduregiCompositor::ivi_controller_screen_set_render_order(QtWaylandServer::ivi_controller_screen::Resource *resource, wl_array *id_layers) {
    (void)resource;
    qDebug() << __func__ << id_layers;
};





void DuduregiCompositor::ivi_controller_bind_resource(QtWaylandServer::ivi_controller::Resource *resource) {
    qDebug() << __func__;
    for(int i=0; i<mGeniviExt->screenCount(); i++) {
        GeniviWaylandIVIExtension::IVIScreen *screen = mGeniviExt->screen(i);

        QtWayland::Output *output = screen->waylandOutput()->handle();
        // find wl_output for client
        if(!output->resourceMap().contains(resource->client()))
            return;
        // get wl_resource of wl_output for client
        QtWaylandServer::wl_output::Resource *output_resource = output->resourceMap().value(resource->client());

        // init screen interface for client
        QtWaylandServer::ivi_controller_screen::init(resource->handle->client, 0, 1);
        // send screen(client, r-id of output for screen, screen) 
        QtWaylandServer::ivi_controller::send_screen(resource->handle, wl_resource_get_id(output_resource->handle), QtWaylandServer::ivi_controller_screen::resource()->handle);

    }


    // XXX should reverse ???
    for(int i=0; i<mGeniviExt->screenCount(); i++) {
        GeniviWaylandIVIExtension::IVIScreen *screen = mGeniviExt->screen(i);
        for(int j=0; j<screen->layerCount(); j++) {
            GeniviWaylandIVIExtension::IVILayer *layer = screen->layer(j);
            qDebug() << "send layer id" << layer->id();

            // send layer
            QtWaylandServer::ivi_controller::send_layer(resource->handle, layer->id());
            for(int k=0; k<layer->surfaceCount(); k++) {
                GeniviWaylandIVIExtension::IVISurface *surface = layer->surface(j);
                qDebug() << "send surface id" << surface->id();

                // send layer
                QtWaylandServer::ivi_controller::send_surface(resource->handle, surface->id());
            }
        }
    }
};
void DuduregiCompositor::ivi_controller_destroy_resource(QtWaylandServer::ivi_controller::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_commit_changes(QtWaylandServer::ivi_controller::Resource *resource) {
    (void)resource;
    qDebug() << __func__;
};
void DuduregiCompositor::ivi_controller_layer_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_layer, int32_t width, int32_t height, uint32_t id) {
    (void)resource;
    qDebug() << __func__ << id_layer << width << height << id;
    QtWaylandServer::ivi_controller_layer::init(resource->handle->client, id, 1);
    struct wl_resource *resource_ctrllayer = QtWaylandServer::ivi_controller_layer::resource()->handle;
    qDebug() << __func__ << "find layer";
    for(int i=0; i<mGeniviExt->mainScreen()->layerCount(); i++) {
        GeniviWaylandIVIExtension::IVILayer *layer = mGeniviExt->mainScreen()->layer(i);
        if(layer->id() == (int)id_layer) {
            qDebug() << "send layer" << id_layer;
            ivi_controller_layer::send_opacity(resource_ctrllayer, layer->opacity());
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

            // send screen
            QtWaylandServer::ivi_controller_layer::send_screen(resource_ctrllayer, output_resource->handle);
            break;
        }
    }
};
void DuduregiCompositor::ivi_controller_surface_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_surface, uint32_t id) {
    (void)resource;
    qDebug() << __func__ << id_surface << id;
    QtWaylandServer::ivi_controller_surface::init(resource->handle->client, id, 1);
};
#endif
