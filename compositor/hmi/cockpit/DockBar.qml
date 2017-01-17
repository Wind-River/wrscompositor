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
import "hmi-interface.js" as Interface

Item {
    id: dockbar
    width: parent.width
    height: parent.height

    FontLoader { id: tungsten; source: "fonts/Tungsten-Light.otf" }

    Rectangle {
        id: dockbarBackground
        color: "gray"
        anchors.fill: parent
    }
    ListModel {
        id: apps
        ListElement {
            label: "Navigation"
            status: "off"
            iconPath: "resources/navigation.png"
        }
        ListElement {
            label: "Climate"
            status: "off"
            iconPath: "resources/climate.png"
            iconScale: 0.6
        }
        ListElement {
            label: "Media"
            status: "off"
            iconPath: "resources/media.png"
            iconScale: 0.9
        }
        ListElement {
            label: "Phone"
            status: "off"
            iconPath: "resources/phone.png"
            iconScale: 0.6
        }
        ListElement {
            label: "Applications"
            status: "off"
            iconPath: "resources/applications.png"
        }
        ListElement {
            label: "Diagnostics"
            status: "off"
            iconPath: "resources/diagnostics.png"
        }
        ListElement {
            label: "Settings"
            status: "off"
            iconPath: "resources/settings.png"
        }
    }
    ListView {
        id: dockview
        model: apps
        anchors.fill: parent
        orientation: Qt.Horizontal
        delegate:  Item {
            id: dockIcon
            width: dockbar.width / 7
            height: parent.height
            Item {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                height: icon.height
                Image {
                    id: icon
                    source: iconPath
                    width: dockIcon.height * 0.8
                    height: width
                    anchors.horizontalCenter: parent.horizontalCenter
                    fillMode: Image.PreserveAspectFit
                    smooth: true;
                    scale: (buttonArea.pressed? 0.9 : 1.0)
                }
            }
            MouseArea {
                id: buttonArea
                anchors.fill: parent
                onClicked: {
                    console.log("clicked: " + label);
                    if (status == "off") {
                        apps.setProperty(index, "status", "on");
                        if (label == "Applications") {
                            Interface.showLauncherWindow();
                        } else {
                            Interface.launchNative(label);
                        }
                    } else {
                        apps.setProperty(index, "status", "off");
                        if (label == "Applications") {
                            Interface.hideLauncherWindow();
                        }
                    }
                }
            }
        }
    }

    function eventHandler(event, object) {
        switch(event) {
            case Interface.COMPOSITOR_EVENT.ADD_WINDOW:
                console.log("DockBar, eventHandler receive ADD_WINDOW event");
                break;
            case Interface.COMPOSITOR_EVENT.REMOVE_WINDOW:
                console.log("DockBar, eventHandler receive REMOVE_WINDOW event");
                break;
            default:
                return;
        }
    }

    Component.onCompleted: {
        /* hmi-interface.js's API: each QML for HMI should register object id and event handler */
        Interface.registerComponent(dockbar, "DockBar");
        Interface.registerNotifyEventHandler(dockbar.eventHandler);
    }
}
