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
    property variant playlist: null
    property int track_index: -1
    property string soundcast_clientid: "c21ebb25eb644205d93434032620af47"

    width : parent.width
    height : parent.height

    FontLoader { id: tungsten; source: "fonts/Tungsten-Light.otf" }
    Rectangle {
        color: "red";
        anchors.fill: parent
    }

    Component.onCompleted: {
    }
    Text {
        id: label
        text: "TEMPERATURE"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: root.top
        anchors.topMargin: root.width / 20
        font.pointSize: root.width / 30
        font.family: tungsten.name
        font.bold: true
        color: "white"
        smooth: true
    }

}


