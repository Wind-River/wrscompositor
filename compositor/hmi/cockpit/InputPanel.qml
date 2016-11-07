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

import QtQuick 2.0
import QtQuick.Window 2.0

Item {
    id: inputpanel
    property bool active: false

    MouseArea {
        anchors.fill: parent
        onClicked: Qt.inputMethod.hide()

    }
    KeyModel {
        id:keyModel
    }

    FontLoader { id: tungsten; source: "fonts/Tungsten-Light.otf" }

    QtObject {
        id:pimpl
        property bool shiftModifier: false
        property bool symbolModifier: false
        property int verticalSpacing: 10
        property int horizontalSpacing: 5
        property int rowHeight: keyboard.height/4 - verticalSpacing
        property int buttonWidth:  (keyboard.width-column.anchors.margins)/10 - horizontalSpacing
    }

    Component {
        id: keyButtonDelegate
        KeyButton {
            width: pimpl.buttonWidth
            height: pimpl.rowHeight
            font.family: tungsten.name
            text: (pimpl.shiftModifier) ? letter.toUpperCase() : (pimpl.symbolModifier)?firstSymbol : letter
            onClicked: virutalKeyboard.sendKeyCode(text)
        }
    }

    Rectangle {
        id:keyboard
        color:"black"
        width: parent.width
        height: parent.height
        anchors.bottom: parent.bottom
        MouseArea {
            anchors.fill: parent
        }

        Column {
            id:column
            anchors.margins: 5
            anchors.fill: parent
            spacing: pimpl.verticalSpacing

            Row {
                height: pimpl.rowHeight
                spacing: pimpl.horizontalSpacing
                anchors.horizontalCenter:parent.horizontalCenter
                Repeater {
                    model: keyModel.firstRowModel
                    delegate: keyButtonDelegate
                }
            }
            Row {
                height: pimpl.rowHeight
                spacing: pimpl.horizontalSpacing
                anchors.horizontalCenter:parent.horizontalCenter
                Repeater {
                    model: keyModel.secondRowModel
                    delegate: keyButtonDelegate
                }
            }
            Item {
                height: pimpl.rowHeight
                width:parent.width
                KeyButton {
                    id: shift
                    anchors.left: parent.left
                    width: 1.53 * pimpl.buttonWidth
                    height: pimpl.rowHeight
                    color: "#1e1b18"
                    Image {
                        source: (pimpl.shiftModifier)? "resources/shift-green.svg" : "resources/shift-gray.svg"
                        scale: (shift.buttonPressed ? 0.95 : 1.0)
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: sourceSize.width
                        height: width
                        smooth: true
                    }
                    onClicked: {
                        if (pimpl.symbolModifier) {
                            pimpl.symbolModifier = false
                        }
                        pimpl.shiftModifier = !pimpl.shiftModifier
                    }
                }
                Row {
                    height: pimpl.rowHeight
                    spacing: pimpl.horizontalSpacing
                    anchors.horizontalCenter:parent.horizontalCenter
                    Repeater {
                        anchors.horizontalCenter: parent.horizontalCenter
                        model: keyModel.thirdRowModel
                        delegate: keyButtonDelegate
                    }
                }
                KeyButton {
                    id: backspace
                    anchors.right: parent.right
                    width: 1.53 * pimpl.buttonWidth
                    height: pimpl.rowHeight
                    color: "#1e1b18"
                    Image {
                        source: "resources/backspace.svg";
                        scale: (backspace.buttonPressed ? 0.95 : 1.0)
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: sourceSize.width * 0.1
                        height: width
                        smooth: true
                    }
                    onClicked: virutalKeyboard.sendSpecialKeyCode("backspace")
                }
            }
            Row {
                height: pimpl.rowHeight
                spacing: pimpl.horizontalSpacing
                anchors.horizontalCenter:parent.horizontalCenter
                KeyButton {
                    id: hide
                    width: 1.35 * pimpl.buttonWidth
                    height: pimpl.rowHeight
                    color: "#1e1b18"
                    Image {
                        source: "resources/hidekeyboard.svg";
                        scale: (hide.buttonPressed ? 0.95 : 1.0)
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: sourceSize.width * 0.1
                        height: width
                        smooth: true
                    }
                    onClicked: {
                        console.log("press hide button in virtualkeyboard");
                        inputpanel.active = false;
                        if (!statusBar.fullscreenViewed)
                            dockBar.show();
                    }
                }
                KeyButton {
                    id: language
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    color: "#1e1b18"
                    Image {
                        source: "resources/global.svg";
                        scale: (language.buttonPressed ? 0.95 : 1.0)
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: sourceSize.width * 0.1
                        height: width
                        smooth: true
                    }
                }
                KeyButton {
                    width: pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: ","
                    onClicked: virutalKeyboard.sendKeyCode(text)
                }
                KeyButton {
                    width: 3*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: " "
                    onClicked: virutalKeyboard.sendKeyCode(text)
                }
                KeyButton {
                    width: pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: "."
                    onClicked: virutalKeyboard.sendKeyCode(text)
                }
                KeyButton {
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    text: (!pimpl.symbolModifier)? "&123" : "ABC"
                    color: "#1e1b18"
                    font.family: tungsten.name
                    onClicked: {
                        if (pimpl.shiftModifier) {
                            pimpl.shiftModifier = false
                        }
                        pimpl.symbolModifier = !pimpl.symbolModifier
                    }
                }
                KeyButton {
                    id: enter
                    width: 1.25*pimpl.buttonWidth
                    height: pimpl.rowHeight
                    color: "#1e1b18"
                    Image {
                        source: "resources/enter.svg";
                        scale: (enter.buttonPressed ? 0.95 : 1.0)
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        width: sourceSize.width * 0.1
                        height: width
                        smooth: true
                    }
                    onClicked: virutalKeyboard.sendSpecialKeyCode("enter")
                }
            }
        }
    }
}
