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

import QtQuick.Window 2.0
import "config.js" as Conf

Item {
    id: container;

    property variant surfaceItem: null
    property variant surface: null
    property variant iviSurface: null
    property int layerId : -1
    property string surfaceName: ""

    signal positionChanged(int x, int y)
    signal sizeChanged(int width, int height)
    signal positionAligned(string align, variant anchors)

    onPositionAligned: {
        switch (align) {
            case 'top':
                container.anchors.top = anchors;
                break;
            case 'bottom':
                container.anchors.bottom = anchors;
                break;
            case 'left':
                container.anchors.left = anchors;
                break;
            case 'right':
                container.anchors.right = anchors;
                break;
            case 'horizontalCenter':
                container.anchors.horizontalCenter = anchors;
                break;
            case 'verticalCenter':
                container.anchors.verticalCenter = anchors
                break;
            default:
                console.log("onPositionAligned, Invalid align");
        }
        console.log("onPositionAligned, align = ", align);
        console.log("onPositionAligned, container name:" + container.surfaceName);

    }

    onPositionChanged: {
        console.log("onPositionChanged:" + x + "/" + y);
        console.log("onPositionChanged, container name:" + container.surfaceName);
        container.x = x;
        container.y = y;
    }

    onSizeChanged: {
        console.log("onSizedChange, size:" + width + "/" + height);
        console.log("onSizedChange, container name:" + container.surfaceName);
        container.width = width
        container.height = height
    }

    Behavior on x {
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on y {
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on width {
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }

    Behavior on height {
        NumberAnimation { easing.type: Easing.InCubic; duration: 200; }
    }
}
