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

Item {
    id: root
    width: Screen.width
    height: Screen.height

    property string dialstring: ""

    FontLoader { id: tungsten; source: "fonts/Tungsten-Light.otf" }

    Rectangle {
        color: "black";
        anchors.fill: parent
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


