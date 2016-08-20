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

    SidePanelDialer {
        id: dialer
        visible: false;
        anchors.top: media.bottom
    }

    function launchWidget(widgetid) {
        media.widgetMode = !(widgetid=='media');
        hvac.visible = (widgetid == 'hvac');
        dialer.visible = (widgetid == 'dialer');

        if(widgetid == 'media' ||
            widgetid == 'hvac' ||
            widgetid == 'dialer')
            return true;
        return false;
    }

    function hide() {
        sidePanel.state = 'hide';
    }

    function show() {
        sidePanel.state = 'show';
    }

    states: [
        State {
            name: "show"
            PropertyChanges { target: sidePanel; opacity: 1; z: 50000 }
        },
        State {
            name: "hide"
            PropertyChanges { target: sidePanel; opacity: 0; z: -1 }
        }
    ]

    transitions: [
        Transition {
            from: "show"
            to: "hide"
            ParallelAnimation {
                NumberAnimation { target: sidePanel; properties: "z"; duration: 300 }
                NumberAnimation { target: sidePanel; properties: "opacity"; duration: 300 }
            }
        },
        Transition {
            from: "hide"
            to: "show"
            ParallelAnimation {
                NumberAnimation { target: sidePanel; properties: "z"; duration: 500 }
                NumberAnimation { target: sidePanel; properties: "opacity"; duration: 500 }
            }
        }
    ]

    Component.onCompleted: {
        console.log("Set show state when creating sidePanel at first-time");
        sidePanel.show();
    }
}