/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1

import QtQuick.Window 2.0
import "config.js" as Conf

Item {
    id: container
    property variant surfaceItem: null
    property variant surface: null

    x: targetX
    y: targetY

    property bool projectionConnectivityStatus: false
    property variant rootBackground: null
    property variant ivi_surface: null
    property variant chrome: null
    property real targetX
    property real targetY
    property real targetWidth
    property real targetHeight
    property int index
    property string clientPath: "none"

    property real scaledWidth: 1.0
    property real scaledHeight: 1.0
    property string position: "main"
    property bool cloned: false

    transformOrigin: Item.Center
    transform: Scale {
        xScale: container.scaledWidth
        yScale: container.scaledHeight
    }

    function hide() {
        container.state = 'hide';
    }

    function show() {
        container.state = 'show';
    }

    states: [
        State {
            name: "show"
            PropertyChanges { target: container; opacity: 1}
        },
        State {
            name: "hide"
            PropertyChanges { target: container; opacity: 0}
        }
    ]

    transitions: [
        Transition {
            from: "show"
            to: "hide"
            ParallelAnimation {
                NumberAnimation {
                    target: container 
                    properties: "x" 
                    easing.type: Easing.InCubic
                    to: -100000
                    duration: 2000
                }
                NumberAnimation {
                    target: container 
                    properties: "opacity" 
                    duration: 2000
                }
            }
            onRunningChanged: {
                if ((state == "hide") && (!running)) {
                    console.log("onRunningChanged, finish hiding window");
                    container.visible = false;
                }
            }
        },
        Transition {
            from: "hide"
            to: "show"
            ParallelAnimation {
                NumberAnimation {
                    target: container 
                    properties: "x" 
                    easing.type: Easing.InCubic
                    to: 0
                    duration: 1000
                }
                NumberAnimation {
                    target: container 
                    properties: "opacity" 
                    duration: 1000
                }
            }
            onRunningChanged: {
                if ((state == "show") && (running)) {
                    console.log("onRunningChanged, starting showing window");
                    container.x = 10000; // swipe in from left to right
                    container.visible = true;
                }
            }
        }
    ] 

    /*
    Connections {
        target: container.surfaceItem ? container.surfaceItem : null
        onSurfaceDestroyed: {
            console.log("onSurfaceDestroyed");
            container.rootBackground.removeWindow(container);
        }
    }
    */
    Behavior on x {
        enabled: Conf.useMultiWindowFeature
        NumberAnimation { easing.type: Easing.InCubic; duration: 300; }
    }

    Behavior on y {
        enabled: Conf.useMultiWindowFeature
        NumberAnimation { easing.type: Easing.InCubic; duration: 300; }
    }

    Behavior on scaledWidth {
        NumberAnimation { easing.type: Easing.InCubic; duration: 300; }
    }

    Behavior on scaledHeight {
        NumberAnimation { easing.type: Easing.InCubic; duration: 300; }
    }
}
