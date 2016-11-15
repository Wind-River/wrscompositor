/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <olszak.tomasz@gmail.com> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return Tomasz Olszak
 * ----------------------------------------------------------------------------
 */

import QtQuick 2.0

Item {
    id:root
    property color color: "#383533"
    property alias text: txt.text
    property alias font: txt.font
    property alias textColor: txt.color
    property alias buttonPressed: buttonMouseArea.pressed
    signal clicked()
    signal pressed()
    signal released()

    Rectangle {
        anchors.fill: parent
        radius: 10
        scale: (buttonMouseArea.pressed? 0.95 : 1.0)
        color: root.color
        border.color: "gray"
        border.width: 2
        Text {
            id: txt
            anchors.margins: 2
            anchors.fill: parent
            fontSizeMode: Text.Fit
            color: "white"
            font.pixelSize: 500
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent
            onClicked: root.clicked()
            onPressed: root.pressed()
            onReleased: root.released()
        }
    }
}
