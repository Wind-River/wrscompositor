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

#include "config.h"
#include "wrscompositor.h"
#include "wrsivicontroller.h"
#include "wrsiviapplication.h"
#include "wrsivisurface.h"
#include "wrslogging.h"
#include "wrsiviplatformconstants.h"
#include "util.h"

#include <QProcess>
#include <sys/signal.h>

WrsCompositor::WrsCompositor(const QString &display, const QString &program)
#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
        : QWaylandQuickCompositor(display.toUtf8().constData(), DefaultExtensions | SubSurfaceExtension)
              , m_fullscreenSurface(0), mIviScene(0), mProgram(program)

#endif
{
    qInfo() << QString("Starts with WAYLAND_DISPLAY=%1").arg(display);
    setTitle(QLatin1String("Wind River WrsCompositor Wayland Compositor"));
    setResizeMode(QQuickView::SizeRootObjectToView);
    setColor(Qt::black);
    winId();
#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
    addDefaultShell();
    setClientFullScreenHint(true);
    connect(this, SIGNAL(afterRendering()), this, SLOT(sendCallbacks()));
#endif
#if WRSCOMPOSITOR_VIRTUAL_KEYBOARD
    defaultInputDevice()->handle()->setCapabilities(QWaylandInputDevice::Touch | QWaylandInputDevice::Pointer | QWaylandInputDevice::Keyboard);
#endif
}

WrsCompositor::~WrsCompositor() {
    delete mIviController;
}

void WrsCompositor::loadQmlComponent(const QSize &size)
{
    QObject *object = rootObject();

    if (object == NULL) {
        DEBUG() << "Try to load QML Component";
#if WRSCOMPOSITOR_HMI_COCKPIT
        QUrl programUrl = QUrl("qrc:///wrscompositor.qml");
#else
        QUrl programUrl = QUrl("qrc:///main.qml");
        if(qApp->arguments().contains("--debug"))
            programUrl = QUrl("hmi/" WRSCOMPOSITOR_HMI_PROFILE "/main.qml");
        if(!mProgram.isNull())
            programUrl = QUrl(mProgram);
#endif

        QVariant width = QVariant(size.width());
        QVariant height = QVariant(size.height());

        rootContext()->setContextProperty("compositor", this);
        rootContext()->setContextProperty("windowHeight", height);
        rootContext()->setContextProperty("windowWidth", width);

        mMainOutput = static_cast<QWaylandQuickOutput*>(createOutput(this, WRSCOMPOSITOR_MANUFACTURER, WRSCOMPOSITOR_PRODUCT_NAME));
        setPrimaryOutput(mMainOutput);
        //mMainOutput->setGeometry(QRect(0, 0, 1280, 720));
        //mMainOutput->window()->setMaximumSize(QSize(16777215, 16777215));
        //mMainOutput->window()->setMaximumSize(QSize(16777215, 16777215));
        mMainOutput->window()->setFlags(Qt::WindowCloseButtonHint);
        mMainOutput->window()->setFlags(Qt::MSWindowsFixedSizeDialogHint); //Set window to fixed size
        mMainOutput->window()->setFlags(Qt::CustomizeWindowHint); //Set window with no title bar
        mMainOutput->window()->setFlags(Qt::FramelessWindowHint); //Set a frameless window

        if((mIviScene == NULL) && size.width() > 0 && size.height() > 0) {
            // this should be after 'wl_output' created, So this is right place
            mIviScene = new WrsIVIModel::IVIScene(this, size.width(), size.height(), object);
            rootContext()->setContextProperty("iviScene", mIviScene);
        }

        setSource(programUrl);
#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
        // initialize IVI extension interfaces after wl_output is created
        // this will ensure the client will first get a reference to wl_output interface
        // then a reference to ivi_screen
        // otherwise the client might call ivi_screen methods without a connection to a specific output

        mIviController  = new WrsIviController(QWaylandCompositor::handle()->wl_display(), this);
        mIviApplication = new WrsIviApplication(QWaylandCompositor::handle()->wl_display(), this);
        mIviSurface     = new WrsIviSurface(QWaylandCompositor::handle()->wl_display(), this);

        QObject::connect(this, SIGNAL(windowAdded(QVariant)), rootObject(), SLOT(windowAdded(QVariant)));
        QObject::connect(this, SIGNAL(windowResized(QVariant)), rootObject(), SLOT(windowResized(QVariant)));
#endif
        connect(qApp, SIGNAL(focusObjectChanged(QObject*)), this, SLOT(slotFocusObjectChanged(QObject*)));
        QObject::connect(this, SIGNAL(windowDestroyed(QVariant)), rootObject(), SLOT(windowDestroyed(QVariant)));
#if WRSCOMPOSITOR_REARDISPLAY
        QObject::connect(rootObject(), SIGNAL(swapWindowRequested(QVariant)), this, SLOT(slotSwapWindow(QVariant)));
        QObject::connect(rootObject(), SIGNAL(cloneWindowRequested(QVariant)), this, SLOT(slotCloneWindow(QVariant)));
        QObject::connect(rootObject(), SIGNAL(closeClonedWindowRequested(QVariant)), this, SLOT(slotCloseClonedWindow(QVariant)));
        QObject::connect(this, SIGNAL(swappedWindowRestored(QVariant)), rootObject(), SLOT(swappedWindowRestored(QVariant)));
#endif
    } else {
        DEBUG() << "Try to update window size in QML Component";
        QQuickItem *item = qobject_cast<QQuickItem*>(object);
        item->setWidth(size.width());
        item->setHeight(size.height());
    }

    DEBUG() << "Resized window's width = " << size.width();
    DEBUG() << "Resized window's height = " << size.height();
}

#if WRSCOMPOSITOR_REARDISPLAY
void WrsCompositor::setRearDisplay(QQuickView *v) {
    mRearDisplay = qobject_cast<RearDisplay*>(v);
    mRearOutput = qobject_cast<QWaylandQuickOutput*>(createOutput(v, WRSCOMPOSITOR_MANUFACTURER, WRSCOMPOSITOR_PRODUCT_NAME));
    // XXX if do not set geometry to last output, main display will be abnormaly rendered, need to investigation
    DEBUG() << mRearOutput->window()->minimumSize();
    DEBUG() << mRearOutput->window()->maximumSize();
    mRearOutput->setGeometry(QRect(0, 0, 1280, 720));
    //mRearOutput->window()->setMinimumSize(QSize(0, 0));
    //mRearOutput->window()->setMaximumSize(QSize(16777215, 16777215));
    mRearOutput->window()->setFlags(Qt::WindowCloseButtonHint);
}
void WrsCompositor::slotSwapWindow(const QVariant &v) {
    QQuickItem *windowFrame = qobject_cast<QQuickItem*>(v.value<QObject*>());
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(windowFrame->property("surfaceItem").value<QObject*>());
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    surface->setMainOutput(mRearOutput);
    QWaylandSurfaceItem *clonedSurfaceItem = static_cast<QWaylandSurfaceItem*>(createView(surface));
    surface->setMainOutput(mMainOutput);

    qobject_cast<RearDisplay*>(mRearDisplay)->addSwappedWindow(clonedSurfaceItem);
}
void WrsCompositor::restoreSwappedWindow(QWaylandSurfaceItem *surfaceItem) {
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    surface->setMainOutput(mMainOutput);
    emit swappedWindowRestored(QVariant::fromValue(surfaceItem));
}
void WrsCompositor::slotCloneWindow(const QVariant &v) {
    QQuickItem *windowFrame = qobject_cast<QQuickItem*>(v.value<QObject*>());
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(windowFrame->property("surfaceItem").value<QObject*>());
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    surface->setMainOutput(mRearOutput);
    QWaylandSurfaceItem *clonedSurfaceItem = static_cast<QWaylandSurfaceItem*>(createView(surface));
    surface->setMainOutput(mMainOutput);
    qobject_cast<RearDisplay*>(mRearDisplay)->addClonedWindow(clonedSurfaceItem);
    mRearDisplay->update();
}
void WrsCompositor::slotCloseClonedWindow(const QVariant &v) {
    QWaylandSurfaceItem *surfaceItem = qobject_cast<QWaylandSurfaceItem*>(v.value<QObject*>());
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface*>(surfaceItem->surface());
    qobject_cast<RearDisplay*>(mRearDisplay)->closeClonedWindow(surface);
}
#endif

void WrsCompositor::slotFocusObjectChanged(QObject *obj) {
    (void)obj;
}

#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
QWaylandQuickSurface* WrsCompositor::fullscreenSurface() const
{
    return m_fullscreenSurface;
}

Q_INVOKABLE QWaylandSurfaceItem* WrsCompositor::item(QWaylandSurface *surf)
{
    return static_cast<QWaylandSurfaceItem *>(surf->views().first());
}

//void WrsCompositor::destroyWindow(QVariant window) {
//    qvariant_cast<QObject *>(window)->deleteLater();
//}

void WrsCompositor::setFullscreenSurface(QWaylandQuickSurface *surface) {
    if (surface == m_fullscreenSurface)
        return;
    m_fullscreenSurface = surface;
    emit fullscreenSurfaceChanged();
}


void WrsCompositor::sendCallbacks() {
    if (m_fullscreenSurface)
        sendFrameCallbacks(QList<QWaylandSurface *>() << m_fullscreenSurface);
    else
        sendFrameCallbacks(surfaces());
}


void WrsCompositor::keyPressEvent(QKeyEvent *event)
{
    DEBUG() << event << event->nativeScanCode() << event->nativeVirtualKey();
    Qt::KeyboardModifiers m = event->modifiers();
    if(((m&Qt::ControlModifier)==Qt::ControlModifier)) {
        int key = event->key();
        for(int _key = Qt::Key_F1; _key < Qt::Key_F9; _key++) {
            if(key == _key) {
                DEBUG() << QString("Ctrl+(Alt)+F%1").arg(_key-Qt::Key_F1+1);
                //QProcess::execute(QString("bash -c \"kill -%1 `pidof wrscompositor-vt-handler`\"").arg(SIGRTMIN+_key-Qt::Key_F1));
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


void WrsCompositor::resizeEvent(QResizeEvent *event)
{
    QQuickView::resizeEvent(event);
    QWaylandCompositor::setOutputGeometry(QRect(0, 0, width(), height()));

    loadQmlComponent(event->size());
}

#if WRSCOMPOSITOR_HMI_COCKPIT
void WrsCompositor::createIviApplicationSurface(QtWaylandServer::ivi_application::Resource *resource, 
                                uint32_t ivi_id, struct ::wl_resource *surface, uint32_t id) {
    TRACE() << "[BEGIN]";

    QWaylandSurface *qWlsurface = QWaylandSurface::fromResource(surface);
    WrsIviSurface *iviApplicationSurface = new WrsIviSurface(this, surface->client, id, 1);

    QVariant returnedValue;
    QMetaObject::invokeMethod(rootObject(), "waylandIviSurfaceCreated",
        Q_RETURN_ARG(QVariant, returnedValue),
        Q_ARG(QVariant, QVariant::fromValue(qWlsurface)),
        Q_ARG(QVariant, QVariant::fromValue(ivi_id)));

    QObject *parentObject = qvariant_cast<QObject *>(returnedValue);
    if (parentObject == NULL)
        return;

    QVariant parentWidth = parentObject->property("width");
    QVariant parentHeight = parentObject->property("height");

    WrsIVIModel::IVISurface *iviSurface = this->findIVISurfaceByQWaylandSurface(qWlsurface);
    iviSurface->setId(ivi_id);
    iviSurface->setWidth(parentWidth.toInt());
    iviSurface->setHeight(parentHeight.toInt());
    iviSurface->addResourceForClient(surface->client, iviApplicationSurface->resource()->handle);

    connect(qWlsurface, SIGNAL(sizeChanged()), 
        this, SLOT(sizeChanged()));

    TRACE() << "[END]";
}
#endif

void WrsCompositor::surfaceCreated(QWaylandSurface *surface) {
    TRACE() << "[BEGIN]";

#if WRSCOMPOSITOR_HMI_CLASSIC
    WrsIVIModel::IVILayer* layer = mIviScene->mainScreen()->layerById(1000);
    if (layer == NULL)
        return;

    // create a new IVIsurface model
    WrsIVIModel::IVISurface *newIviSurface = layer->addSurface();
    newIviSurface->setQWaylandSurface(surface);

    connect(surface, SIGNAL(windowPropertyChanged(const QString &, const QVariant &)),
            this, SIGNAL(windowPropertyChanged(const QString &, const QVariant &)));
    connect(surface, SIGNAL(sizeChanged()),
            this, SLOT(sizeChanged()));
#endif // WRSCOMPOSITOR_HMI_CLASSIC

    // On surface create add listener for surface events
    connect(surface, SIGNAL(surfaceDestroyed()),
            this, SLOT(surfaceDestroyed()));
    connect(surface, SIGNAL(mapped()),
            this, SLOT(surfaceMapped()));
    connect(surface, SIGNAL(unmapped()),
            this, SLOT(surfaceUnmapped()));

    TRACE() << "[END]";
}


void WrsCompositor::surfaceMapped() {
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());

    emit windowAdded(QVariant::fromValue(surface));
}


void WrsCompositor::surfaceUnmapped() {
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());
    if (surface == m_fullscreenSurface)
        m_fullscreenSurface = 0;
}


void WrsCompositor::surfaceDestroyed() {
    TRACE() << "[BEGIN]";
    QWaylandQuickSurface *surface = static_cast<QWaylandQuickSurface *>(sender());
    WrsIVIModel::IVISurface *iviSurface = this->findIVISurfaceByQWaylandSurface(surface);

    if (iviSurface == NULL) {
        DEBUG() << " There isn't any IVISurface mapped to qwaylandSurface";
        return;
    }

    if (surface == m_fullscreenSurface)
        m_fullscreenSurface = 0;

    WrsIVIModel::IVILayer* layer = iviSurface->getParentLayer();
    if (layer == NULL)
        return;

    // Remove coresponding IVI surface from IVILayer
    layer->removeSurface(iviSurface);

    emit windowDestroyed(QVariant::fromValue(surface));
    TRACE() << "[END]";
}


void WrsCompositor::windowPropertyChanged(const QString &property, const QVariant &value) {
    TRACE() << "[BEGIN]";
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());
    DEBUG() << "Surface:" << surface << " property:" << property << " value:" << value;
    TRACE() << "[END]";
}

void WrsCompositor::sizeChanged() {
    TRACE() << "[BEGIN]";
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());
#if WRSCOMPOSITOR_HMI_COCKPIT
    WrsIVIModel::IVISurface *iviSurface = this->findIVISurfaceByQWaylandSurface(surface);
    DEBUG() << "iviSurface's width = " << iviSurface->width() << " iviSurface's height = " << iviSurface->height();
    //Get IVI surface for QWaylandSurface
    QtWaylandServer::ivi_surface::Resource::fromResource(
        iviSurface->getResourceForClient(surface->client()->client())
        )->ivi_surface_object->send_configure(
        iviSurface->width(), iviSurface->height());
#endif
    TRACE() << "[END]";
}

WrsIVIModel::IVISurface* WrsCompositor::findSurfaceByResource(struct ::wl_resource *rsc) {
    WrsIVIModel::IVISurface *surface = NULL;
    for (int i = 0; i < mIviScene->screenCount(); i++) {
        WrsIVIModel::IVIScreen *screen = mIviScene->screen(i);
        for (int j = 0; j < screen->layerCount(); j++) {
            WrsIVIModel::IVILayer *layer = screen->layer(j);
            for (int k = 0; k < layer->surfaceCount(); k++) {
                WrsIVIModel::IVISurface *_surface = layer->surface(k);
                if (_surface->getResourceForClient(rsc->client) == rsc) {
                    surface = _surface;
                    break;
                }
            }
        }
    }
    return surface;
}

WrsIVIModel::IVISurface* WrsCompositor::findIVISurfaceByQWaylandSurface(QWaylandSurface *qWlSurface) {
    DEBUG() << "search:" << qWlSurface;

    for (int i = 0; i < mIviScene->screenCount(); i++) {
        WrsIVIModel::IVIScreen *screen = mIviScene->screen(i);
        for (int j = 0; j < screen->layerCount(); j++) {
            WrsIVIModel::IVILayer *layer = screen->layer(j);
            for (int k = 0; k < layer->surfaceCount(); k++) {
                WrsIVIModel::IVISurface *surface = layer->surface(k);
                if (surface->qWaylandSurface() == qWlSurface) {
                    DEBUG() << "found:" << (WrsIVIModel::IVISurface* ) surface;
                    return (WrsIVIModel::IVISurface*) surface;
                }
            }
        }
    }
    return NULL;
}

QString WrsCompositor::getSurfaceRole(QWaylandSurface *qWlSurface) {
    WrsIVIModel::IVISurface* iviSurface = findIVISurfaceByQWaylandSurface(qWlSurface);

    if (iviSurface->id() == WRS_IVI_ID_SURFACE_CAMERA) {
        return "Camera";
    } else if (iviSurface->id() == WRS_IVI_ID_SURFACE_DIALOG) {
        return "Dialog";
    } else if (iviSurface->id() == WRS_IVI_ID_SURFACE_NAVIGATION) {
        return "Navigation";
    } else if (iviSurface->id() == WRS_IVI_ID_SURFACE_PHONE) {
        return "Phone";
    } else if (iviSurface->id() == WRS_IVI_ID_SURFACE_PROJECTION) {
        return "Projection";
    } else {
        Util u;
        return u.getCmdForPid(qWlSurface->client()->processId());
    }
}

#endif
