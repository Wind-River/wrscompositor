/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1
import com.windriver.duduregi 1.0

Item {
    id: dockbar
    anchors.bottom: parent.bottom
    width: parent.width
    height: (parent.height*2)/10
    z:  50000
    signal launched(string appid);

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
            appid: "navigation"
            apptype: "native"
            path: "skobblernavi"
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
                    console.log("clicked: "+label);
                    if(apptype=="native") {
                        if(process.pid<0)
                            process.execute(path);
                    } else
                        dockbar.launched(appid);
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