import QtQuick 2.1
import QtQuick.Window 2.0

Item {
    id: container
    property variant child: null

    x: targetX
    y: targetY
    width: targetWidth
    height: targetHeight

    property variant ivi_surface: null
    property variant chrome: null
    property real targetX
    property real targetY
    property real targetWidth
    property real targetHeight
    property int index

    Connections {
        target: container.child ? container.child : null
        onSurfaceDestroyed: {
            container.parent.removeWindow(container);
        }
    }
}
