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
import com.windriver.wrscompositor 1.0
import "config.js" as Conf
import "hmi-controller.js" as Control

Rectangle {
    id: mainMenu
    width: parent.width
    height: parent.height
    anchors.fill: parent
    color: "black"

    property variant currentWindow: null 

    Behavior on opacity {
        NumberAnimation { easing.type: Easing.InCubic; duration: 400; }
    }

    function show() {
        mainMenu.visible = true
        mainMenu.focus = true
        Control.getInstance().hideWindow(currentWindow);
    }

    function hide() {
        mainMenu.visible = false
        mainMenu.focus = true
        Control.getInstance().showWindow(currentWindow);
    }

    function launchNative(appid) {
        for (var i = 0; i < nativeAppsView.count; i++) {
            var app = nativeAppsView.model.get(i);
            if (appid == app.description) {
                console.log("launchNative, app = ", appid);
                var delegateItem = nativeAppsView.getDelegateInstanceAt(i);
                delegateItem.launch();
            }
        }
    }

    ListModel {
        id: nativeApps
        ListElement {
            label: "Web Browsing"
            description: "Web Browsing"
            exec: "/usr/share/qt5/examples/webkitwidgets/browser/browser"
            multiple: false
            iconPath: "icons/native-web.png"
        }
        ListElement {
            label: "Navigation"
            description: "Navigation"
            exec: "/usr/bin/skobblernavi"
            multiple: false
            iconPath: "icons/native-map-location.png"
            unitFile: "skobblernavi.service"
        }
        ListElement {
            label: "Media"
            description: "Media"
            exec: "/usr/bin/mediaplayer"
            multiple: false
            iconPath: "icons/native-video.png"
            unitFile: "mediaplayer.service"
        }
        ListElement {
            label: "Phone"
            description: "Phone"
            exec: "/usr/bin/phone"
            multiple: false
            iconPath: "icons/native-phone.png"
            unitFile: "phone.service"
        }
    }

    Component {
        id: nativeAppsDelegate
        Item {
            id: delegateItem
            property variant process: null
            property bool pressed: false
            width: ((nativeAppsView.cellWidth * 0.8) | 0)
            height: ((nativeAppsView.cellHeight * 0.8) | 0)

            Image {
                id: appIcon
                source: iconPath
                anchors.centerIn: parent
                scale: (pressed||iconMouseArea.pressed)?(parent.height/height):(parent.height/height)*0.9
                MouseArea {
                    id: iconMouseArea
                    anchors.fill: parent
                    onClicked: {
                        nativeAppsView.currentIndex = index;
                        delegateItem.process.launch();
                    }
                }
                Image {
                    id: quitButton
                    visible: delegateItem.process.getPid() != 0
                    enabled: visible
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenterOffset: parent.width*0.4
                    anchors.verticalCenterOffset: -parent.width*0.4
                    source: "icons/close.png"
                    smooth: true
                    width: height
                    height: quitArea.pressed?((parent.height/4.5)|0):((parent.height/4)|0)
                    MouseArea {
                        id: quitArea
                        anchors.fill: parent
                        onClicked: {
                            delegateItem.process.quit();
                        }
                    }
                }
            }
            Text {
                id: myText
                anchors.top: parent.bottom
                text: label
                font.pointSize: ((parent.height / 8.0)/Conf.densityAdjust|0)
                anchors.horizontalCenter: parent.horizontalCenter
                color: "black"
                style: Text.Outline
                styleColor: "white"
                smooth: true
            }

            Component.onCompleted: {
                /* hmi-controller.js's API: create Process object to launch specific application */
                delegateItem.process = Control.getInstance().createProcessUnit(exec);
            }
        }
    }

    GridView {
        id: nativeAppsView
        model: nativeApps
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width*0.8
        height: parent.height*0.8
        cellWidth: ((width/4.0)|0)
        cellHeight: cellWidth
        delegate: nativeAppsDelegate
        highlight: Rectangle {
            color: "lightsteelblue";
            radius: 5
        }

        function getDelegateInstanceAt(index) {
            console.log("getDelegateInstanceAt[" + index + "]");

            var len = contentItem.children.length;
            console.log("getDelegateInstanceAt: len[" + len + "]");

            if (len > 0 && index > -1 && index < len) {
                return contentItem.children[index];
            } else {
                console.log("getDelegateInstanceAt: index[" + index + "] is invalid for len[" + len + "]");
                return undefined;
            }
        }
    }

    function eventHandler(event, object) {
        switch(event) {
            case Control.Event.WindowAdded: {
                console.log("MainMenu, eventHandler receive WindowAdded event");
                mainMenu.currentWindow = object;
                break;
            }

            case Control.Event.WindowRemoved: {
                console.log("MainMenu, eventHandler receive WindowRemoved event");
                if(mainMenu.currentWindow == object)
                    mainMenu.currentWindow = null;
                break;
            }

            default:
                return;
        }
    }

    Component.onCompleted: {
        /* hmi-controller.js's API: each QML for HMI should register object id and event handler */
        Control.getInstance().registerObjectItem(mainMenu, "MainMenu");
        Control.getInstance().registerEventHandler(mainMenu.eventHandler);
    }
}
