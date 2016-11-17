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
import "config.js" as Conf

Item {
    id: sidePanel
    width: parent.width
    height: parent.height

    signal launchWidget(string widgetid);

    onLaunchWidget: {
        media.widgetMode = !(widgetid=='media');
        hvac.visible = (widgetid == 'hvac');
        dialer.visible = (widgetid == 'dialer');

        if(widgetid == 'media' ||
            widgetid == 'hvac' ||
            widgetid == 'dialer')
            return true;
        return false;
    }

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

    Component.onCompleted: {
        Conf.registerObjectItem("SidePanel", sidePanel);
    }
}