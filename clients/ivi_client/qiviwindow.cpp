#include "qiviwindow.h"
#include "qtwaylandiviapplication.h"

static void myRegistryListener(
        void *data,
        struct wl_registry *registry,
        uint32_t id,
        const QString &interface,
        uint32_t version) {

    qDebug() << "INTERFACE !!!" << interface;

    QIVIWindow *iviWindow = (QIVIWindow *) data;
    if (interface == "ivi_application") {
        qDebug() << "    - Registered to ivi_application";
        iviWindow->mIviApp->init(registry, id, version);
    }
    if (interface == "ivi_surface") {
        qDebug() << "    - Registered to ivi_surface";
        iviWindow->mIviSurface->init(registry, id, version);
    }
}


QIVIWindow::QIVIWindow(QtWaylandClient::QtWaylandIviApplication *iviApp, int iviId) : QMainWindow(NULL)
{
    this->mIviApp       = iviApp;
    this->mIviID        = iviId;
    this->mIviSurface   = new QtWaylandClient::QtWaylandIviSurface();
    this->mIviSurfaceInst = NULL;
}


bool QIVIWindow::event(QEvent *event) {
    bool ret = QMainWindow::event(event);
    qDebug() << "Event:" << event;

    if (this->windowHandle() != NULL) {
        QtWaylandClient::QWaylandWindow  *w = (QtWaylandClient::QWaylandWindow  *) this->windowHandle()->handle();
        if (w != NULL) {
            if (w->screen() != 0) {
                if (w->isInitialized()) {
                    if (this->mIviApp->isInitialized()) {
                        if (this->mIviSurfaceInst == NULL) {
                            qDebug() << "    - Create IVI surface";
                            this->mIviSurfaceInst = new QtWaylandClient::QtWaylandIviSurface(
                                    this->mIviApp->surface_create(
                                            this->mIviID,
                                            w->object()));
                        }
                    } else {
                        if (w->display() != NULL) {
                            qDebug() << "    - Register to ivi_application interface";
                            w->display()->addRegistryListener(
                                    &myRegistryListener,
                                    this);
                        }
                    }
                }
            }
        }
    }
    return ret;
}
