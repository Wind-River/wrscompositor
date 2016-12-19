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
import QtMultimedia 5.0
import QtGraphicalEffects 1.0
import "wrscompositor.js" as Logic
import "hmi-controller.js" as Control
import "config.js" as Conf

Item {
    id: root
    signal sendEvent(int event, var object);
    property var hmiController: null
    property var compositorLogic : null

    x: 0
    y: 0
    height: windowHeight
    width: windowWidth

    property bool hasFullscreenWindow: typeof compositor != "undefined" && compositor.fullscreenSurface !== null

    onHasFullscreenWindowChanged: {
        console.log("has fullscreen window: " + hasFullscreenWindow);
    }

    Component.onCompleted: {
        compositorLogic = Logic.getInstance();
        hmiController = Control.getInstance();

        if (hmiController) {
            hmiController.setRootObject(root);
        }

        if (compositorLogic) {
            compositorLogic.setHmiController(hmiController);
            compositorLogic.setIviScene(iviScene);
            compositorLogic.setWrsCompositor(compositor);
            compositorLogic.setDisplaySize(Conf.displayWidth, Conf.displayHeight);
            compositorLogic.init();
        }
    }

    onWidthChanged: {
        Conf.displayWidth = width;
    }
    onHeightChanged: {
        Conf.displayHeight = height;
    }

    function waylandIviSurfaceCreated(surface, id) {
        console.log("surface created, id = ", id); 
        return compositorLogic.createWaylandIviSurface(surface, id);
    }

    function windowDestroyed(surface) {
        var windowFrame = compositorLogic.findBySurface(surface);
        if (!windowFrame) {
            console.log("windowDestroyed, cannot find surface in windowList");
            return;
        }

        console.log('window destroyed '+ surface);
        windowFrame.destroy();

        compositorLogic.removeWindow(windowFrame);

        if (windowFrame) {
            console.log("notify WindowRemoved event");
            root.sendEvent(Control.Event.WindowRemoved, windowFrame);
        }
    }

    function windowAdded(surface) {
        console.log('surface added ' + surface);
        console.log('surface added title:' + surface.title);
        console.log('surface added className:' + surface.className);
        console.log('surface added width: ' + surface.size.width);
        console.log('surface added height: ' + surface.size.height);
        console.log(iviScene.mainScreen);
        console.log(iviScene.mainScreen.layerCount());

        var windowFrame = compositorLogic.addSurface(surface);

        if (windowFrame) {
            console.log("notify WindowAdded event of each qml components for HMI");
            root.sendEvent(Control.Event.WindowAdded, windowFrame);
        }
    }
}
