/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
#include "duduregiconfig.h"
#include "duduregicompositor.h"
#include <QProcess>
#include <sys/signal.h>

DuduregiCompositor::DuduregiCompositor(const QString &display, const QString &program)
#if DUDUREGI_WAYLAND_COMPOSITOR
        : QWaylandQuickCompositor(display.toUtf8().constData(), DefaultExtensions | SubSurfaceExtension)
        //, QtWaylandServer::ivi_controller_surface(QWaylandCompositor::handle()->wl_display(), 1)
        //, QtWaylandServer::ivi_controller_layer(QWaylandCompositor::handle()->wl_display(), 1)
        //, QtWaylandServer::ivi_controller_screen(QWaylandCompositor::handle()->wl_display(), 1)
              , m_fullscreenSurface(0), mGeniviExt(0), mProgram(program)

#endif
{
    qInfo() << QString("Starts with WAYLAND_DISPLAY=%1").arg(display);
    setTitle(QLatin1String("Wind River Duduregi Wayland Compositor"));
    setResizeMode(QQuickView::SizeRootObjectToView);
    setColor(Qt::black);
    winId();
#if DUDUREGI_WAYLAND_COMPOSITOR
    addDefaultShell();
    setClientFullScreenHint(true);
    connect(this, SIGNAL(afterRendering()), this, SLOT(sendCallbacks()));

    QtWaylandServer::ivi_controller::init(QWaylandCompositor::handle()->wl_display(), 1);
#endif
}

DuduregiCompositor::~DuduregiCompositor() {
}

void DuduregiCompositor::loadQmlComponent(const QSize &size)
{
    QObject *object = rootObject();

    if (object == NULL) {
        qDebug() << "Try to load QML Component";
        QUrl programUrl = QUrl("qrc:///main.qml");
        if(qApp->arguments().contains("--debug"))
            programUrl = QUrl("hmi/" DUDUREGI_HMI_PROFILE "/main.qml");
        if(!mProgram.isNull())
            programUrl = QUrl(mProgram);

        QVariant width = QVariant(size.width());
        QVariant height = QVariant(size.height());

        rootContext()->setContextProperty("compositor", this);
        rootContext()->setContextProperty("windowHeight", height);
        rootContext()->setContextProperty("windowWidth", width);

        setSource(programUrl);

        mMainOutput = static_cast<QWaylandQuickOutput*>(createOutput(this, DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME));
        setPrimaryOutput(mMainOutput);
        //mMainOutput->setGeometry(QRect(0, 0, 1280, 720));
        //mMainOutput->window()->setMaximumSize(QSize(16777215, 16777215));
        //mMainOutput->window()->setMaximumSize(QSize(16777215, 16777215));
        mMainOutput->window()->setFlags(Qt::WindowCloseButtonHint);
        mMainOutput->window()->setFlags(Qt::MSWindowsFixedSizeDialogHint); //Set window to fixed size
        mMainOutput->window()->setFlags(Qt::CustomizeWindowHint); //Set window with no title bar
        mMainOutput->window()->setFlags(Qt::FramelessWindowHint); //Set a frameless window

#if DUDUREGI_WAYLAND_COMPOSITOR
        QObject::connect(this, SIGNAL(windowAdded(QVariant)), rootObject(), SLOT(windowAdded(QVariant)));
        QObject::connect(this, SIGNAL(windowResized(QVariant)), rootObject(), SLOT(windowResized(QVariant)));
#endif
        connect(qApp, SIGNAL(focusObjectChanged(QObject*)), this, SLOT(slotFocusObjectChanged(QObject*)));
        QObject::connect(this, SIGNAL(windowDestroyed(QVariant)), rootObject(), SLOT(windowDestroyed(QVariant)));
#if DUDUREGI_REARDISPLAY
        QObject::connect(rootObject(), SIGNAL(swapWindowRequested(QVariant)), this, SLOT(slotSwapWindow(QVariant)));
        QObject::connect(rootObject(), SIGNAL(cloneWindowRequested(QVariant)), this, SLOT(slotCloneWindow(QVariant)));
        QObject::connect(rootObject(), SIGNAL(closeClonedWindowRequested(QVariant)), this, SLOT(slotCloseClonedWindow(QVariant)));
        QObject::connect(this, SIGNAL(swappedWindowRestored(QVariant)), rootObject(), SLOT(swappedWindowRestored(QVariant)));
#endif
    } else {
        qDebug() << "Try to update window size in QML Component";
        QQuickItem *item = qobject_cast<QQuickItem*>(object);
        item->setWidth(size.width());
        item->setHeight(size.height());
    }

    qDebug() << "Resized window's width = " << size.width();
    qDebug() << "Resized window's height = " << size.height();
}

#if DUDUREGI_REARDISPLAY
void DuduregiCompositor::setRearDisplay(QQuickView *v) {
    mRearDisplay = qobject_cast<RearDisplay*>(v);
    mRearOutput = qobject_cast<QWaylandQuickOutput*>(createOutput(v, DUDUREGI_MANUFACTURER, DUDUREGI_PRODUCT_NAME));
    // XXX if do not set geometry to last output, main display will be abnormaly rendered, need to investigation
    qDebug() << mRearOutput->window()->minimumSize();
    qDebug() << mRearOutput->window()->maximumSize();
    mRearOutput->setGeometry(QRect(0, 0, 1280, 720));
    //mRearOutput->window()->setMinimumSize(QSize(0, 0));
    //mRearOutput->window()->setMaximumSize(QSize(16777215, 16777215));
    mRearOutput->window()->setFlags(Qt::WindowCloseButtonHint);
}
void DuduregiCompositor::slotSwapWindow(const QVariant &v) {
    QQuickItem *windowFrame = qobject_cast<QQuickItem*>(v.value<QObject*>());
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(windowFrame->property("surfaceItem").value<QObject*>());
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    surface->setMainOutput(mRearOutput);
    QWaylandSurfaceItem *clonedSurfaceItem = static_cast<QWaylandSurfaceItem*>(createView(surface));
    surface->setMainOutput(mMainOutput);

    qobject_cast<RearDisplay*>(mRearDisplay)->addSwappedWindow(clonedSurfaceItem);
}
void DuduregiCompositor::restoreSwappedWindow(QWaylandSurfaceItem *surfaceItem) {
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    surface->setMainOutput(mMainOutput);
    emit swappedWindowRestored(QVariant::fromValue(surfaceItem));
}
void DuduregiCompositor::slotCloneWindow(const QVariant &v) {
    QQuickItem *windowFrame = qobject_cast<QQuickItem*>(v.value<QObject*>());
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(windowFrame->property("surfaceItem").value<QObject*>());
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    surface->setMainOutput(mRearOutput);
    QWaylandSurfaceItem *clonedSurfaceItem = static_cast<QWaylandSurfaceItem*>(createView(surface));
    surface->setMainOutput(mMainOutput);
    qobject_cast<RearDisplay*>(mRearDisplay)->addClonedWindow(clonedSurfaceItem);
    mRearDisplay->update();
}
void DuduregiCompositor::slotCloseClonedWindow(const QVariant &v) {
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(v.value<QObject*>());
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    qobject_cast<RearDisplay*>(mRearDisplay)->closeClonedWindow(surface);
}
#endif

void DuduregiCompositor::slotFocusObjectChanged(QObject *obj) {
    (void)obj;
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

Q_INVOKABLE QString DuduregiCompositor::getProcessNameByPid(int pid) {
#define MAX_PROCESS_NAME 512
    char name[MAX_PROCESS_NAME] = {0,};

    sprintf(name, "/proc/%d/cmdline", pid);
    FILE* fp = fopen(name, "r");
    if (!fp)
        return QString::null;

    int size = fread(name, sizeof(char), MAX_PROCESS_NAME, fp);
    if (size < 0 || size == 0)
        return QString::null;

    name[size] = '\0';
    QString processName(name);

    return processName;
}

//void DuduregiCompositor::destroyWindow(QVariant window) {
//    qvariant_cast<QObject *>(window)->deleteLater();
//}

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

    emit windowDestroyed(QVariant::fromValue(surface));
}

void DuduregiCompositor::sendCallbacks() {
    if (m_fullscreenSurface)
        sendFrameCallbacks(QList<QWaylandSurface *>() << m_fullscreenSurface);
    else
        sendFrameCallbacks(surfaces());
}

void DuduregiCompositor::keyPressEvent(QKeyEvent *event)
{
    qDebug() << __func__ << event << event->nativeScanCode() << event->nativeVirtualKey();
    Qt::KeyboardModifiers m = event->modifiers();
    if(((m&Qt::ControlModifier)==Qt::ControlModifier)) {
        int key = event->key();
        for(int _key = Qt::Key_F1; _key < Qt::Key_F9; _key++) {
            if(key == _key) {
                qDebug() << QString("Ctrl+(Alt)+F%1").arg(_key-Qt::Key_F1+1);
                //QProcess::execute(QString("bash -c \"kill -%1 `pidof duduregi-vt-handler`\"").arg(SIGRTMIN+_key-Qt::Key_F1));
                QProcess::execute(QString("dbus-send --system --dest=org.freedesktop.login1 --type=method_call --print-reply /org/freedesktop/login1/seat/seat0  org.freedesktop.login1.Seat.SwitchTo uint32:%1").arg(_key-Qt::Key_F1+1));
                event->ignore();
                return;
            }
        }
        if(key == Qt::Key_W) {
            qApp->quit();
            event->ignore();
            return;
        }
    }
    QQuickView::keyPressEvent(event);
}

void DuduregiCompositor::resizeEvent(QResizeEvent *event)
{
    QQuickView::resizeEvent(event);
    QWaylandCompositor::setOutputGeometry(QRect(0, 0, width(), height()));

    loadQmlComponent(event->size());

    if(!mGeniviExt && width() > 0 && height() > 0) {
        // this should be after 'wl_output' created, So this is right place
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
GeniviWaylandIVIExtension::IVISurface* DuduregiCompositor::findSurfaceByResource(struct ::wl_resource *rsc) {
    GeniviWaylandIVIExtension::IVISurface *surface = NULL;
    for(int i=0; i<mGeniviExt->screenCount(); i++) {
        GeniviWaylandIVIExtension::IVIScreen *screen = mGeniviExt->screen(i);
        for(int j=0; j<screen->layerCount(); j++) {
            GeniviWaylandIVIExtension::IVILayer *layer = screen->layer(j);
            for(int k=0; k<layer->surfaceCount(); k++) {
                GeniviWaylandIVIExtension::IVISurface *_surface = layer->surface(k);
                if(_surface->waylandResource() == rsc) {
                    surface = _surface;
                    break;
                }
            }
        }
    }
    return surface;
}

void DuduregiCompositor::ivi_controller_surface_set_visibility(QtWaylandServer::ivi_controller_surface::Resource *resource, uint32_t visibility) {
    (void)resource;
    qDebug() << __func__ << visibility;
    GeniviWaylandIVIExtension::IVISurface *surface = findSurfaceByResource(resource->handle);
    if(!surface)
        return;
    qDebug() << "surface id " << surface->id() << visibility;
    surface->setVisibility(visibility);
    surface->qmlWindowFrame()->setProperty("visible", visibility);
};
void DuduregiCompositor::ivi_controller_surface_set_opacity(QtWaylandServer::ivi_controller_surface::Resource *resource, wl_fixed_t opacity) {
    (void)resource;
    qDebug() << __func__ << opacity;
    GeniviWaylandIVIExtension::IVISurface *surface = findSurfaceByResource(resource->handle);
    if(!surface)
        return;
    qDebug() << "surface id " << surface->id() << opacity;
    surface->setOpacity(wl_fixed_to_double(opacity));
    surface->qmlWindowFrame()->setProperty("opacity", wl_fixed_to_double(opacity));
};
void DuduregiCompositor::ivi_controller_surface_set_source_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << x << y << width << height;
};
void DuduregiCompositor::ivi_controller_surface_set_destination_rectangle(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t x, int32_t y, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << x << y << width << height;
    GeniviWaylandIVIExtension::IVISurface *surface = findSurfaceByResource(resource->handle);
    if(!surface)
        return;
    qDebug() << "surface id " << surface->id() << x << y << width << height;
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
};
void DuduregiCompositor::ivi_controller_surface_set_configuration(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t width, int32_t height) {
    (void)resource;
    qDebug() << __func__ << width << height;
    GeniviWaylandIVIExtension::IVISurface *surface = findSurfaceByResource(resource->handle);
    if(!surface)
        return;
    surface->qmlWindowFrame()->setProperty("width", width);
    surface->qmlWindowFrame()->setProperty("height", height);
    surface->setWidth(width);
    surface->setHeight(height);
};
void DuduregiCompositor::ivi_controller_surface_set_orientation(QtWaylandServer::ivi_controller_surface::Resource *resource, int32_t orientation) {
    (void)resource;
    qDebug() << __func__ << orientation;
    GeniviWaylandIVIExtension::IVISurface *surface = findSurfaceByResource(resource->handle);
    if(!surface)
        return;
    switch(orientation) {
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
    if(orientation>=0 && orientation<4)
        surface->setOrientation(orientation);
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
                GeniviWaylandIVIExtension::IVISurface *surface = layer->surface(k);
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
            layer->setWaylandResource(resource_ctrllayer);
            qDebug() << "send layer" << id_layer;
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
};
void DuduregiCompositor::ivi_controller_surface_create(QtWaylandServer::ivi_controller::Resource *resource, uint32_t id_surface, uint32_t id) {
    (void)resource;
    qDebug() << __func__ << id_surface << id;
    GeniviWaylandIVIExtension::IVISurface *surface = NULL;
    GeniviWaylandIVIExtension::IVILayer *parentLayer = NULL;
    for(int i=0; i<mGeniviExt->screenCount(); i++) {
        GeniviWaylandIVIExtension::IVIScreen *screen = mGeniviExt->screen(i);
        for(int j=0; j<screen->layerCount(); j++) {
            GeniviWaylandIVIExtension::IVILayer *layer = screen->layer(j);
            for(int k=0; k<layer->surfaceCount(); k++) {
                GeniviWaylandIVIExtension::IVISurface *_surface = layer->surface(k);
                if(_surface->id() == (int)id_surface) {
                    surface = _surface;
                    parentLayer = layer;
                    qDebug() << "found surface " << id << surface;
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

};
#endif
