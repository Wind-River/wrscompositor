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
import "config.js" as Conf
import com.windriver.automotive 1.0
import com.windriver.genivi 1.0

Rectangle {
    id: rearRoot
    color: "#ff0000"
    anchors.fill: parent
    property variant currentWindow: null
    property var clonedSurfaceItemList: null;

    signal swappedWindowRestoreRequested(var anObject)
    signal clonedWindowRestoreRequested(var anObject)
    signal clonedSurfaceItemDestroyed(var anObject)

    Rectangle {
        id: statusBar
        anchors.top: parent.top
        width: parent.width
        height: parent.height/12
        z: 50000
        property bool currentWindowExposed: rearRoot.currentWindow && rearRoot.currentWindow.visible

        Rectangle {
            id: statusbarBackground
            color: "#2e2e2e"
            anchors.fill: parent
        }

        Image {
            id: logo_title
            source: "images/wr-red.png"
            height: statusBar.height * 0.7
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            width: (height*sourceSize.width)/sourceSize.height
            smooth: true
        }
        Text {
            id: wmi_title
            anchors.left: logo_title.right
            anchors.verticalCenter: parent.verticalCenter
            text: "Rear Monitor"
            font.pointSize: ((statusBar.height*0.4/Conf.densityAdjust)|0)
            color: "white"
            smooth: true
            font.bold: true
        }



        Image {
            id: swapWindowButton
            source: "icons/restore-swapped-window.svg"
            anchors.right: dateTime.left
            anchors.rightMargin: width
            anchors.verticalCenter: parent.verticalCenter
            visible: Conf.useMultiWaylandDisplayFeature && statusBar.currentWindowExposed
            width: height
            height: parent.height * 3 / 5
            MouseArea {
                id: swapButtonArea
                anchors.fill: parent
                onClicked: {
                    if(!rearRoot.currentWindow)
                        return;
                    rearRoot.swappedWindowRestoreRequested(rearRoot.currentWindow);
                    rearRoot.currentWindow = null;
                }
            }
            opacity: (swapButtonArea.pressed?0.8:1.0)
            scale: (swapButtonArea.pressed?0.9:1.0)
            smooth: true
        }

        Text {
            id: dateTime
            anchors.right: parent.right
            anchors.rightMargin: width/20
            anchors.verticalCenter: parent.verticalCenter
            text: Qt.formatDateTime(new Date(), "yyyy/MM/dd hh:mm:ss")
            font.pointSize: ((statusBar.height*0.4/Conf.densityAdjust)|0)
            color: "white"
            smooth: true
        }

        Timer {
            interval: 1000; running: true; repeat: true
            onTriggered: dateTime.text = Qt.formatDateTime(new Date(), "yyyy/MM/dd hh:mm:ss")
        }

    }

    Image {
        id: background
        anchors.top: statusBar.bottom
        width: parent.width
        height: parent.height - statusBar.height
        Behavior on opacity {
            NumberAnimation { easing.type: Easing.InCubic; duration: 400; }
        }
        fillMode: Image.PreserveAspectCrop
        source: "alameda.jpg"
        smooth: true
    }

    function windowSwapped(surfaceItem) {
        console.log("swapped window added: "+surfaceItem);
        surfaceItem.parent = background
        rearRoot.currentWindow = surfaceItem
        if (rearRoot.clonedSurfaceItemList == null)
            rearRoot.clonedSurfaceItemList = new Array(0);
        rearRoot.clonedSurfaceItemList.push(surfaceItem);
    }
    function windowCloned(surfaceItem) {
        console.log("cloned: "+surfaceItem);
        surfaceItem.parent = background
        if (rearRoot.clonedSurfaceItemList == null)
            rearRoot.clonedSurfaceItemList = new Array(0);
        rearRoot.clonedSurfaceItemList.push(surfaceItem);
    }
    function windowCloneClosed(surface) {
        console.log("cloned closed: "+surface);
        var i;
        console.log("====================================");
        for (i = 0; i < rearRoot.clonedSurfaceItemList.length; i++) {
            var surfaceItem = rearRoot.clonedSurfaceItemList[i];
            console.log("check "+surfaceItem.surface+" "+surface);
            if(surfaceItem.surface == surface) {
                console.log("removed"+surface);
                rearRoot.clonedSurfaceItemList.splice(i, 1);
                rearRoot.clonedSurfaceItemDestroyed(surfaceItem);
                surfaceItem.parent = null; // delete
                break;
            }
        }
        console.log("====================================");
    }
    Component.onCompleted: {
    }
}
