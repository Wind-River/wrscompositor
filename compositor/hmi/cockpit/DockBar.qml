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
import "hmi-controller.js" as Control
import com.windriver.wrscompositor 1.0

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
            appid: "Navigation"
            iconPath: "resources/navigation.png"
        }
        ListElement {
            label: "Climate"
            appid: "Climate"
            iconPath: "resources/climate.png"
            iconScale: 0.6
        }
        ListElement {
            label: "Media"
            appid: "Media"
            iconPath: "resources/media.png"
            iconScale: 0.9
        }
        ListElement {
            label: "Phone"
            appid: "Phone"
            iconPath: "resources/phone.png"
            iconScale: 0.6
        }
        ListElement {
            label: "Applications"
            appid: "Applications"
            iconPath: "resources/applications.png"
        }
        ListElement {
            label: "Diagnostics"
            appid: "Diagnostics"
            iconPath: "resources/diagnostics.png"
        }
        ListElement {
            label: "Settings"
            appid: "settings"
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
                    var mainmenu =  Control.getInstance().findObjectByName("MainMenu");

                    if (appid == 'Applications') {
                        if (mainmenu.visible) mainmenu.hide()
                        else mainmenu.show()
                    } else {
                        mainmenu.launchNative(appid);
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        Control.getInstance().registerObjectItem(dockbar, "DockBar");
    }
}
