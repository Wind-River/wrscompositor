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

Rectangle {
    id: launcherWindow
    color: Qt.rgba(0.6, 0.3, 0.3, 0.1)
    width: parent.width
    height: parent.height

    ListModel {
        id: appsx
        ListElement {
            name: ""
            icon: "icons/climate.png"
            status: "off"
            url: "http://www.gmail.com"
        }
        ListElement {
            name: ""
            icon: "icons/diagnostics.png"
            status: "off"
            url: "https://www.google.com/maps/"
        }
        ListElement {
            name: ""
            icon: "icons/settings.png"
            status: "off"
            url: "https://play.google.com/store/music"
        }
        ListElement {
            name: ""
            icon: "icons/climate.png"
            status: "off"
            url: "https://play.google.com/store/music"
        }
    }

    GridView {
        id: gridx
        model: appsx
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width*0.8
        height: parent.height*0.8
        cellWidth: ((width/4.0)|0)
        cellHeight: cellWidth
        delegate:  Rectangle {
            color: "transparent"
            property bool pressed: false
            width: ((gridx.cellWidth * 0.8) | 0)
            height: ((gridx.cellHeight * 0.8) | 0)
            Image {
                source: icon
            }
//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    if (status == "off") {
//                        appsx.setProperty(index, "status", "on");
//                        output.destoryCompositorElement("LauncherWindow");
//                        output.createWebViewToCompositorElement(url, name);
//                    } else {
//                        appsx.setProperty(index, "status", "off");
//                        output.destoryCompositorElement(name);
//                    }
//                }
//            }
        }
    }
}
