import QtQuick 2.0
import QtQuick.Window 2.0

Item {
    id: container

    x: targetX
    y: targetY
    width: targetWidth
    height: targetHeight
    scale: targetScale

    visible: isFullscreen || !root.hasFullscreenWindow
    onVisibleChanged: {
        child.clientRenderingEnabled = visible
        console.log("visibility changed: " + visible);
    }

    opacity: 0

    property real targetX
    property real targetY
    property real targetWidth
    property real targetHeight
    property real targetScale

    property variant child: null
    property variant chrome: null
    property bool animationsEnabled: false
    property bool isFullscreen: state === "fullscreen"
    property int index

    state: child && chrome && chrome.selected && child.focus ? "fullscreen" : "normal"

    Behavior on x {
        enabled: container.animationsEnabled;
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on y {
        enabled: container.animationsEnabled;
        NumberAnimation { easing.type: Easing.InQuad; duration: 200; }
    }

    Behavior on width {
        enabled: container.animationsEnabled;
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on height {
        enabled: container.animationsEnabled;
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on scale {
        enabled: container.animationsEnabled;
        NumberAnimation { easing.type: Easing.InQuad; duration: 200; }
    }

    Behavior on opacity {
        enabled: true;
        NumberAnimation { easing.type: Easing.Linear; duration: 250; }
    }

    ContrastEffect {
        id: effect
        source: child
        anchors.fill: child
        blend: { if (child && chrome && (chrome.selected || child.focus)) 0.0; else 0.6 }
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

    property real fullscreenScale: Math.min(root.width / width, root.height / height)

    transitions: [
        Transition {
            from: "*"; to: "normal"
            SequentialAnimation {
                ScriptAction {
                    script: {
                        compositor.fullscreenSurface = null
                        background.opacity = 1
                    }
                }
                ParallelAnimation {
                    NumberAnimation { target: container; property: "x"; easing.type: Easing.Linear; to: targetX; duration: 400; }
                    NumberAnimation { target: container; property: "y"; easing.type: Easing.Linear; to: targetY; duration: 400; }
                    NumberAnimation { target: container; property: "scale"; easing.type: Easing.Linear; to: targetScale; duration: 400; }
                }
                ScriptAction {
                    script: container.z = 0
                }
            }
        },
        Transition {
            from: "*"; to: "fullscreen"
            SequentialAnimation {
                ScriptAction {
                    script: {
                        container.z = 1
                        background.opacity = 0
                    }
                }
                ParallelAnimation {
                    NumberAnimation { target: container; property: "x"; easing.type: Easing.Linear; to: (root.width - container.width) / 2; duration: 400; }
                    NumberAnimation { target: container; property: "y"; easing.type: Easing.Linear; to: (root.height - container.height) / 2; duration: 400; }
                    NumberAnimation { target: container; property: "scale"; easing.type: Easing.Linear; to: fullscreenScale; duration: 400; }
                }
                ScriptAction {
                    script: compositor.fullscreenSurface = child.surface
                }
            }
        }
    ]

    SequentialAnimation {
        id: destroyAnimation
        NumberAnimation { target: scaleTransform; property: "yScale"; easing.type: Easing.Linear; to: 0.01; duration: 200; }
        NumberAnimation { target: scaleTransform; property: "xScale"; easing.type: Easing.Linear; to: 0.01; duration: 150; }
        NumberAnimation { target: container; property: "opacity"; easing.type: Easing.Linear; to: 0.0; duration: 150; }
        ScriptAction { script: container.parent.removeWindow(child); }
    }

    function runDestroyAnimation() {
        destroyAnimation.start();
    }

    Image {
        source: "closebutton.png"
        smooth: true

        opacity: !isFullscreen && chrome && chrome.selected ? 1 : 0
        Behavior on opacity {
            NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
        }

        x: parent.width - 32
        y: 4
        width: 24
        height: 24
        z: 4

        MouseArea {
            anchors.fill: parent
            onClicked: {
                child.surface.destroySurface()
            }
        }
    }
}
