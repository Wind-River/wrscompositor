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

/**
 * Represents the compositor window frame
 * Layout
 */
Rectangle {
    id: compositorElement;
    property variant    subItem:            null;
    property string     subItemName:        "";
    property variant    subItemSurface:     null;
    property variant    subItemIVISurface:  null;
    property string     prefferedPosition:  "";

    signal newSize (int x, int y, int width, int height)


    //border.color: Qt.rgba(0.3, 0.3, 0.3, 0.3)
    //border.width: 2
    color: Qt.rgba(0.3, 0.3, 0.3, 0.1)

    Component.onCompleted: {
        console.log("onCompleted:" + compositorElement.prefferedPosition)
        //output.windowAdded(compositorElement);
    }

    onNewSize: {
        console.log("[DEBUG] onNewSize:" + width + "/" + height);
        console.log("[DEBUG] onNewPos:" + x + "/" + y);
        console.log("[DEBUG] compositorElement name:" + compositorElement.subItemName);
        //xanimation.to = x;
        //yanimation.to = y;
        //wanimation.to = width;
        //hanimation.to = height;
        //xanimation.start();
        //yanimation.start();
        //wanimation.start();
        //hanimation.start();
        compositorElement.x = x;
        compositorElement.y = y;
        compositorElement.width = width
        compositorElement.height = height
        if (subItemSurface != null) {
            subItemSurface.requestSize(Qt.size(width, height));
        }
    }

    NumberAnimation on x {
        target: compositorElement
        id: xanimation;
        properties: "x"
        running: false
        duration: 100
        onRunningChanged: {
            if(!xanimation.running) {
                compositorElement.x = xanimation.to;
            }
        }
    }
    NumberAnimation on y {
        target: compositorElement
        id: yanimation;
        properties: "y"
        running: false
        duration: 100
        onRunningChanged: {
            if(!xanimation.running) {
                compositorElement.y = yanimation.to;
            }
        }
    }
    NumberAnimation on width {
        target: compositorElement
        id: wanimation;
        properties: "width"
        running: false
        duration: 100
    }
    NumberAnimation on height {
        target: compositorElement
        id: hanimation;
        properties: "height"
        running: false
        duration: 100
    }
}
