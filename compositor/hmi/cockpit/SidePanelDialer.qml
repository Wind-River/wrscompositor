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

    function hide() {
        root.state = 'hide';
    }

    function show() {
        root.state = 'show';
    }

    states: [
        State {
            name: "show"
            PropertyChanges { target: root; opacity: 1}
        },
        State {
            name: "hide"
            PropertyChanges { target: root; opacity: 0}
        }
    ]

    transitions: [
        Transition {
            from: "show"
            to: "hide"
            ParallelAnimation {
                NumberAnimation {
                    target: root
                    properties: "opacity"
                    easing.type: Easing.InCubic
                    duration: 300
                }
            }
            onRunningChanged: {
                if ((state == "hide") && (!running)) {
                    console.log("onRunningChanged, finish hiding dialer's widget");
                    root.visible = false;
                }
            }
        },
        Transition {
            from: "hide"
            to: "show"
            ParallelAnimation {
                NumberAnimation {
                    target: root
                    properties: "x"
                    easing.type: Easing.InCubic
                    to: 0
                    duration: 500
                }
                NumberAnimation {
                    target: root
                    properties: "opacity"
                    duration: 500
                }
            }
            onRunningChanged: {
                if ((state == "show") && (running)) {
                    console.log("onRunningChanged, starting showing dialer's widget");
                    root.x = 700; // swipe in from right to left
                    root.visible = true;
                }
            }
        }
    ]

    Component.onCompleted: {
        root.hide();
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
        interactive: false
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


