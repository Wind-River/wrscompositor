#include "qtwaylandivisurface.h"


namespace QtWaylandClient {


    QtWaylandIviSurface::~QtWaylandIviSurface() {

    }


    void QtWaylandIviSurface::ivi_surface_configure(int32_t width, int32_t height) {
        qDebug("<<< configure <<< %d/%d", width, height);
    }
}
