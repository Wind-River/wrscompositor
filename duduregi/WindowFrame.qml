import QtQuick 2.1
import QtQuick.Window 2.0

Item {
    id: container
    property variant child: null

    x: targetX
    y: targetY
    width: targetWidth
    height: targetHeight
    scale: targetScale

    property variant chrome: null
    property real targetX
    property real targetY
    property real targetWidth
    property real targetHeight
    property real targetScale
    property int index

    ContrastEffect {
        id: effect
        source: child
        anchors.fill: parent
        opacity: 1.0
        z: 1

        Behavior on blend {
            enabled: true;
            NumberAnimation { easing.type: Easing.Linear; duration: 200; }
        }
    }

    transform: [
        Scale { id: scaleTransform; origin.x: container.width / 2; origin.y: container.height / 2; xScale: 1; yScale: 1 }
    ]

    SequentialAnimation {
        id: destroyAnimation
        NumberAnimation { target: scaleTransform; property: "yScale"; easing.type: Easing.Linear; to: 0.01; duration: 200; }
        NumberAnimation { target: scaleTransform; property: "xScale"; easing.type: Easing.Linear; to: 0.01; duration: 150; }
        NumberAnimation { target: container; property: "opacity"; easing.type: Easing.Linear; to: 0.0; duration: 150; }
    }
    function runDestroyAnimation() {
        destroyAnimation.start();
    }

    Connections {
	target: container.child ? container.child : null
	onSurfaceDestroyed: {
	    container.parent.removeWindow(container);
	}
    }
}
