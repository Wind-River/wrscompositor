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

import QtQuick 2.2
import QtQuick.Window 2.2

FocusScope {
    id: focusScope;
    //width: 400;
    //height: textInput.paintedHeight + (2 * textInput.anchors.topMargin);
    width: Screen.width
    height: Screen.height


    property alias  value                          : textInput.text;
    property alias  fontSize                       : textInput.font.pointSize;
    property alias  textColor                      : textInput.color;
    property alias  placeHolder                    : typeSomething.text;

    Rectangle {
        id: background;
        anchors.fill: parent;
        color: "#AAAAAA";
        radius: 5;
        antialiasing: true;
        border {
            width: 3;
            color: (focusScope.activeFocus ? "red" : "steelblue");
        }
    }
    TextEdit {
        id: textInput;
        focus: true
        selectByMouse: true
        font.pointSize: 20;
        wrapMode: TextEdit.WrapAtWordBoundaryOrAnywhere;
        color: "black";
        anchors {
            top: parent.top;
            topMargin: 10;
            left: parent.left;
            leftMargin: 10;
            right: parent.right;
            rightMargin: 10;
        }
    }
    Text {
        id: typeSomething;
        text: "Type something...";
        color: "gray";
        opacity: (value === "" ? 1.0 : 0.0);
        font {
            italic: true
            pointSize: fontSize;
        }
        anchors {
            left: parent.left;
            right: parent.right;
            leftMargin: 10;
            rightMargin: 10;
            verticalCenter: parent.verticalCenter;
        }

        Behavior on opacity { NumberAnimation { duration: 250; } }
    }
    MouseArea {
        visible: (!focusScope.activeFocus);
        anchors.fill: parent
        onClicked: { textInput.forceActiveFocus(); }
    }
    Text {
        id: clear;
        text: "\u2717" // 'x' //"\u2713"
        color: 'steelblue';
        font.pixelSize: 30;
        opacity: (value !== "" ? 1.0 : 0.0);
        anchors {
            right: parent.right;
            bottom: parent.bottom;
            margins: 5;
        }

        Behavior on opacity { NumberAnimation { duration: 250; } }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                value = "";
                focusScope.focus = false;
            }
        }
    }
}
