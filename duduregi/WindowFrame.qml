import QtQuick 2.0
import QtQuick.Window 2.0

Item {
    id: container
    property variant child: null

    ContrastEffect {
        id: effect
        source: child
        anchors.fill: child
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
}
