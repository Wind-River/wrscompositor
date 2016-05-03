/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1

Item {
    id: dockbar
    anchors.bottom: parent.bottom
    width: parent.width
    height: (parent.height*2)/10
    z: 50000
    signal launch(string appid);

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
            iconPath: "resources/navi.svg"
        }
        ListElement {
            label: "Climate"
            appid: "hvac"
            iconPath: "resources/hvac.svg"
            iconScale: 0.6
        }
        ListElement {
            label: "Media"
            appid: "media"
            iconPath: "resources/mobile.svg"
            iconScale: 0.9
        }
        ListElement {
            label: "Phone"
            appid: "dialer"
            iconPath: "resources/phone.svg"
            iconScale: 0.6
        }
        ListElement {
            label: "Applications"
            appid: "menu"
            iconPath: "resources/apps.svg"
        }
        ListElement {
            label: "Diagnostics"
            appid: "diagnostics"
            iconPath: "resources/diagnotics.svg"
        }
        ListElement {
            label: "Settings"
            appid: "settings"
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
                    dockbar.launch(appid);
                }
            }
        }

    }


}

