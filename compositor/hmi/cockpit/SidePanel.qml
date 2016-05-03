/*
 * Copyright © 2016 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */
import QtQuick 2.1

Item {
    id: sidePanel
    width: parent.width*0.34
    height: parent.height*0.7
    z: 50000

    Rectangle {
        id: panelBackground
        color: "black"
        anchors.fill: parent
    }

    SidePanelMedia {
        id: media
        visible: true;
    }

    SidePanelHVAC {
        id: hvac
        visible: true;
        anchors.top: media.bottom
    }

    function launchWidget(widgetid) {
        media.widgetMode = !(widgetid=='media');
        return false;
    }


}

