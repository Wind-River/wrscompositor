/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1

Item {
    id: root
    property string icon: ""
    property double innerScale: 0.5

    Image {
        id: ring
        source: "resources/ring.svg"
        anchors.fill: parent
        smooth: true
    }
    Image {
        id: inner
        source: icon
        anchors.horizontalCenter: ring.horizontalCenter
        anchors.verticalCenter: ring.verticalCenter
        width: (sourceSize.width * height)/sourceSize.height
        height: ring.height * innerScale
    }

}
