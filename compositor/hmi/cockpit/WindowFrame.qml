/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
    property int processId: 0
    property string projectionName: "none"

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
                    duration: 3000
                }
                NumberAnimation {
                    target: container 
                    properties: "opacity" 
                    duration: 3000
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
                    container.x = 1000; // swipe in from left to right
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
