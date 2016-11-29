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

import QtQuick 2.1;
import QtGraphicalEffects 1.0;

/**
 * Represents the compositor window frame
 * Layout
 */
Rectangle {
    id: compositorLayer;
    property variant    subItemIVILayer:  null;
    x: 0
    y: 0
    width: 1855
    height: 1056

    color: Qt.rgba(0.0, 0.0, 0.0, 0.0);

    signal newSize (int x, int y, int width, int height)
    signal transparencyMode (int mode)

    MouseArea {
        id: inputEventsMask
        anchors.fill: parent;
    }

    Component.onCompleted: {
        console.log("onCompleted: !!!!")
        //output.windowAdded(compositorElement);
    }

    onNewSize: {
    }

    onTransparencyMode: {
        if (mode == 0) {//fully transparent
            inputEventsMask.enabled = false;
            color = Qt.rgba(0.0, 0.0, 0.0, 0.0);
        }
        if (mode == 1) {//semi transparent but let events
            inputEventsMask.enabled = false;
            color = Qt.rgba(0.0, 0.0, 0.0, 0.5);
            opacity = 1.0;
        }
        if (mode == 2) {//semi transparent but don't let events
            inputEventsMask.enabled = true;
            color = Qt.rgba(0.0, 0.0, 0.0, 0.5);
            opacity = 1.0;
        }
        if (mode == 3) {//opaque and don't let events
            inputEventsMask.enabled = true;
            color = Qt.rgba(0.0, 0.0, 0.0, 1.0);
            opacity = 1.0;
        }
    }
}
