/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
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
