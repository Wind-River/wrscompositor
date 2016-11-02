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

    mIviController  = new WrsIviController(QWaylandCompositor::handle()->wl_display(), this);
    mIviApplication = new WrsIviApplication(QWaylandCompositor::handle()->wl_display(), this);
    mIviSurface     = new WrsIviSurface(QWaylandCompositor::handle()->wl_display(), this);
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
        qDebug() << "Try to load QML Component";
        QUrl programUrl = QUrl("qrc:///main.qml");
        if(qApp->arguments().contains("--debug"))
            programUrl = QUrl("hmi/" WRSCOMPOSITOR_HMI_PROFILE "/main.qml");
        if(!mProgram.isNull())
            programUrl = QUrl(mProgram);

        QVariant width = QVariant(size.width());
        QVariant height = QVariant(size.height());

        rootContext()->setContextProperty("compositor", this);
        rootContext()->setContextProperty("windowHeight", height);
        rootContext()->setContextProperty("windowWidth", width);

        setSource(programUrl);

        mMainOutput = static_cast<QWaylandQuickOutput*>(createOutput(this, WRSCOMPOSITOR_MANUFACTURER, WRSCOMPOSITOR_PRODUCT_NAME));
        setPrimaryOutput(mMainOutput);
        //mMainOutput->setGeometry(QRect(0, 0, 1280, 720));
        //mMainOutput->window()->setMaximumSize(QSize(16777215, 16777215));
        //mMainOutput->window()->setMaximumSize(QSize(16777215, 16777215));
        mMainOutput->window()->setFlags(Qt::WindowCloseButtonHint);
        mMainOutput->window()->setFlags(Qt::MSWindowsFixedSizeDialogHint); //Set window to fixed size
        mMainOutput->window()->setFlags(Qt::CustomizeWindowHint); //Set window with no title bar
        mMainOutput->window()->setFlags(Qt::FramelessWindowHint); //Set a frameless window

#if WRSCOMPOSITOR_WAYLAND_COMPOSITOR
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
        qDebug() << "Try to update window size in QML Component";
        QQuickItem *item = qobject_cast<QQuickItem*>(object);
        item->setWidth(size.width());
        item->setHeight(size.height());
    }

    qDebug() << "Resized window's width = " << size.width();
    qDebug() << "Resized window's height = " << size.height();
}

#if WRSCOMPOSITOR_REARDISPLAY
void WrsCompositor::setRearDisplay(QQuickView *v) {
    mRearDisplay = qobject_cast<RearDisplay*>(v);
    mRearOutput = qobject_cast<QWaylandQuickOutput*>(createOutput(v, WRSCOMPOSITOR_MANUFACTURER, WRSCOMPOSITOR_PRODUCT_NAME));
    // XXX if do not set geometry to last output, main display will be abnormaly rendered, need to investigation
    qDebug() << mRearOutput->window()->minimumSize();
    qDebug() << mRearOutput->window()->maximumSize();
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

Q_INVOKABLE QString WrsCompositor::getProcessPathByPid(int pid) {
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
    qDebug() << __func__ << event << event->nativeScanCode() << event->nativeVirtualKey();
    Qt::KeyboardModifiers m = event->modifiers();
    if(((m&Qt::ControlModifier)==Qt::ControlModifier)) {
        int key = event->key();
        for(int _key = Qt::Key_F1; _key < Qt::Key_F9; _key++) {
            if(key == _key) {
                qDebug() << QString("Ctrl+(Alt)+F%1").arg(_key-Qt::Key_F1+1);
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

    if(!mIviScene && width() > 0 && height() > 0) {
        // this should be after 'wl_output' created, So this is right place
        mIviScene = new WrsIVIModel::IVIScene(this, width(), height(), this);
        rootContext()->setContextProperty("geniviExt", mIviScene);
    }
}


void WrsCompositor::surfaceCreated(QWaylandSurface *surface) {
    TRACE() << "[BEGIN]";

    // create a new IVIsurface model
    WrsIVIModel::IVISurface *newIviSurface = new WrsIVIModel::IVISurface(this);
    // link the IVISurface model to wl_surface(or QWaylandSurface)
    newIviSurface->setQWaylandSurface(surface);
    // add the surface to the scene model
    mIviScene->addIVISurface(newIviSurface);

    // On surface create add listener for surface events
    connect(surface, SIGNAL(surfaceDestroyed()),
            this, SLOT(surfaceDestroyed()));
    connect(surface, SIGNAL(mapped()),
            this, SLOT(surfaceMapped()));
    connect(surface, SIGNAL(unmapped()),
            this, SLOT(surfaceUnmapped()));
    connect(surface, SIGNAL(windowPropertyChanged(const QString &, const QVariant &)),
            this, SIGNAL(windowPropertyChanged(const QString &, const QVariant &)));
    connect(surface, SIGNAL(sizeChanged()),
            this, SLOT(sizeChanged()));
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
    QWaylandQuickSurface *surface = static_cast<QWaylandQuickSurface *>(sender());
    if (surface == m_fullscreenSurface)
        m_fullscreenSurface = 0;

    emit windowDestroyed(QVariant::fromValue(surface));
}


void WrsCompositor::windowPropertyChanged(const QString &property, const QVariant &value) {
    QWaylandQuickSurface *surface = qobject_cast<QWaylandQuickSurface *>(sender());
    qDebug() << "Surface:" << surface << " property:" << property << " value:" << value;
}


void WrsCompositor::sizeChanged() {
    //TODO: do we need to do something here?
}

#endif
