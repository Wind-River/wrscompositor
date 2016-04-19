/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1

Item {
    id: chrome
    anchors.fill: parent

    property variant window: parent;
    property bool selected: root.selectedWindow === window

    MouseArea {
        anchors.fill: parent
        enabled: !window.focus
        hoverEnabled: !window.focus
        onClicked: {
            if (selected) {
                window.takeFocus();
            } else {
                root.selectedWindow = window
                root.focus = true
            }
        }
    }
}
