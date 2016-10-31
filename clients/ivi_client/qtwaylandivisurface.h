#ifndef QTWAYLANDIVISURFACE_H
#define QTWAYLANDIVISURFACE_H

#include <QObject>
#include <QtWaylandClient>
#include <QtWaylandClient/private/qwaylandwindow_p.h>
#include <QtWaylandClient/private/qwaylanddisplay_p.h>
#include "qwayland-ivi-application.h"

namespace QtWaylandClient {
    class QtWaylandIviSurface : public QObject, public QtWayland::ivi_surface
    {
    public:
        QtWaylandIviSurface(struct ::wl_registry *registry, int id, int version)
                : ivi_surface(registry, id, version)
        {

        }

        QtWaylandIviSurface(struct ::ivi_surface *object)
                : ivi_surface(object)
        {

        }

        QtWaylandIviSurface() {

        }

        ~QtWaylandIviSurface();

        Q_OBJECT
    protected:
        void ivi_surface_configure(int32_t width, int32_t height);
    };
}
#endif // QTWAYLANDIVISURFACE_H
