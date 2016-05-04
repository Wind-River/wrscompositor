/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1
import QtMultimedia 5.0
import QtGraphicalEffects 1.0
import "sprintf.js" as SPrintf

Item {
    id: root
    width : parent.width
    height : parent.height
    property string dialstring: ""

    FontLoader { id: tungsten; source: "fonts/Tungsten-Light.otf" }
    Rectangle {
        color: "#222222";
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 3
    }
    /*
    Rectangle {
        color: "black";
        anchors.fill: parent
        border.width: width/20
        border.color: "red"
    }
    */

    Component.onCompleted: {
    }

    ListModel {
        id: dialModel
        ListElement {
            label: "1"
        }
        ListElement {
            label: "2"
        }
        ListElement {
            label: "3"
        }
        ListElement {
            label: "4"
        }
        ListElement {
            label: "5"
        }
        ListElement {
            label: "6"
        }
        ListElement {
            label: "7"
        }
        ListElement {
            label: "8"
        }
        ListElement {
            label: "9"
        }
        ListElement {
            label: "*"
        }
        ListElement {
            label: "0"
        }
        ListElement {
            label: "#"
        }
    }

    Text {
        id: label
        text: dialstring==""?"PHONE":dialstring
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: root.top
        anchors.topMargin: root.width / 20
        font.pointSize: root.width / 30
        font.family: tungsten.name
        font.bold: true
        color: "white"
        smooth: true
    }

    GridView {
        id: grid
        model: dialModel
        anchors.top: label.bottom
        anchors.topMargin: label.height / 2
        anchors.left: parent.left
        anchors.leftMargin: parent.width/4
        anchors.right: parent.right
        anchors.rightMargin: parent.width/4
        height: cellHeight*4
        cellWidth: width/3
        cellHeight: parent.width/8
        delegate:  Item {
            width: grid.cellWidth
            height: grid.cellHeight
            Rectangle {
                width: grid.cellHeight - grid.cellHeight/10
                height: width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                radius: 100
                color: dialarea.pressed?"#3BBDED":"black"
                border.width: width/40
                border.color: "white"
                Text {
                    text: label
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    font.pointSize: parent.width / 2
                    font.family: "Monospace"
                    font.bold: true
                    color: "white"
                    smooth: true
                }
                MouseArea {
                    id: dialarea
                    anchors.fill: parent
                    onClicked: {
                        dialstring = dialstring + label
                    }
                }
            }
        }
    }

    onVisibleChanged: {
        if(!visible)
            dialstring = "";
    }
}


