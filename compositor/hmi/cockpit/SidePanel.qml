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
        console.log("launchWidget, widgetid = " + widgetid);

        switch (widgetid) {
            case 'media':
                media.show();
                hvac.hide();
                dialer.hide();
                break;
            case 'hvac':
                hvac.show();
                media.hide();
                dialer.hide();
                break;
            case 'dialer':
                dialer.show();
                media.hide();
                hvac.hide();
                break;
            default:
                return false;
        }

        return true;
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