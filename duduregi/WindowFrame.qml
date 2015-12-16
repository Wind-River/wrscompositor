import QtQuick 2.1
import QtQuick.Window 2.0
import "config.js" as Conf

Item {
    id: container
    property variant child: null

    x: targetX
    y: targetY

    property bool androidAutoProjection: false
    property variant rootBackground: null
    property variant ivi_surface: null
    property variant chrome: null
    property real targetX
    property real targetY
    property real targetWidth
    property real targetHeight
    property int index

    property real scaledWidth: 1.0
    property real scaledHeight: 1.0

    transformOrigin: Item.Center
    transform: Scale {
        xScale: container.scaledWidth
        yScale: container.scaledHeight
    }

    Connections {
        target: container.child ? container.child : null
        onSurfaceDestroyed: {
            container.rootBackground.removeWindow(container);
        }
    }
    Behavior on x {
		enabled: Conf.useMultiWindowFeature
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on y {
		enabled: Conf.useMultiWindowFeature
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on scaledWidth {
		enabled: Conf.useMultiWindowFeature
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on scaledHeight {
		enabled: Conf.useMultiWindowFeature
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }


}
