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

Item {
    id: dockbar
    anchors.bottom: parent.bottom
    width: parent.width
    height: (parent.height*2)/12
    z:  50000
    signal launched(string apptype, string appid);

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
            appid: "mocknavi"
            apptype: "native"
            iconPath: "resources/navi.svg"
        }
        ListElement {
            label: "Climate"
            appid: "hvac"
            apptype: "widget"
            iconPath: "resources/hvac.svg"
            iconScale: 0.6
        }
        ListElement {
            label: "Media"
            appid: "media"
            apptype: "widget"
            iconPath: "resources/mobile.svg"
            iconScale: 0.9
        }
        ListElement {
            label: "Phone"
            appid: "dialer"
            apptype: "widget"
            iconPath: "resources/phone.svg"
            iconScale: 0.6
        }
        ListElement {
            label: "Applications"
            appid: "menu"
            apptype: "widget"
            iconPath: "resources/apps.svg"
        }
        ListElement {
            label: "Diagnostics"
            appid: "diagnostics"
            apptype: "widget"
            iconPath: "resources/diagnotics.svg"
        }
        ListElement {
            label: "Settings"
            appid: "settings"
            apptype: "widget"
            iconPath: "resources/settings.svg"
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
                height: icon.height+iconLabel.height
                RingIcon {
                    id: icon
                    icon: iconPath
                    innerScale: iconScale?iconScale:0.5
                    width: buttonArea.pressed?dockIcon.height*3/5:dockIcon.height/2
                    height: width
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                Text {
                    id: iconLabel
                    text: label.toUpperCase()
                    anchors.top: icon.bottom
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pointSize: buttonArea.pressed?dockIcon.height/7:dockIcon.height/8
                    font.family: tungsten.name
                    font.bold: true
                    color: "white"
                    smooth: true
                }
            }
            MouseArea {
                id: buttonArea
                anchors.fill: parent
                onClicked: {
                    console.log("clicked: " + label);
                    dockbar.launched(apptype, appid);
                }
            }
        }
    }

    function hide() {
        dockbar.state = 'hide';
    }

    function show() {
        dockbar.state = 'show';
    }

    states: [
        State {
            name: "show"
            PropertyChanges { target: dockbar; opacity: 1; z: 50000 }
        },
        State {
            name: "hide"
            PropertyChanges { target: dockbar; opacity: 0; z: -1 }
        }
    ]

    transitions: [
        Transition {
            from: "show"
            to: "hide"
            ParallelAnimation {
                NumberAnimation { target: dockbar; properties: "z"; duration: 300 }
                NumberAnimation { target: dockbar; properties: "opacity"; duration: 300 }
            }
        },
        Transition {
            from: "hide"
            to: "show"
            ParallelAnimation {
                NumberAnimation { target: dockbar; properties: "z"; duration: 500 }
                NumberAnimation { target: dockbar; properties: "opacity"; duration: 500 }
            }
        }
    ]

    Component.onCompleted: {
        console.log("Set show state when creating dockbar at first-time");
        dockbar.show();
    }
}
