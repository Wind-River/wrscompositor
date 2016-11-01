#ifndef WRSIVISURFACE_H
#define WRSIVISURFACE_H

#include "wrscompositor.h"

/**
 * @brief The WrsIviSurface class
 *        Associate a wl_surface to an ivi_surface
 */
class WrsIviSurface
        : public QObject
        , public QtWaylandServer::ivi_surface
{
    Q_OBJECT
private:
    WrsCompositor *mCompositor;

public:
    WrsIviSurface(struct ::wl_display *display, WrsCompositor *compositor);
    WrsIviSurface(WrsCompositor *compositor, struct ::wl_client *client, int id, int version);

protected:
    void ivi_surface_bind_resource(QtWaylandServer::ivi_surface::Resource *resource);
    void ivi_surface_destroy_resource(QtWaylandServer::ivi_surface::Resource *resource);
    void ivi_surface_destroy(QtWaylandServer::ivi_surface::Resource *resource);

private:

public:
};

#endif // WRSIVISURFACE_H
