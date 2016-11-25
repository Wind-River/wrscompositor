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
import "compositor.js" as Logic
import "config.js" as Conf
import "sprintf.js" as SPrintf
import com.windriver.automotive 1.0
import com.windriver.genivi 1.0

Item {
    id: root
    property var compositorLogic : null

    x: 0
    y: 0
    height: windowHeight
    width: windowWidth

    property variant waitProcess: null
    property bool hasFullscreenWindow: typeof compositor != "undefined" && compositor.fullscreenSurface !== null

    signal addedWaylandSurfaceWindow(variant surface);

    onHasFullscreenWindowChanged: {
        console.log("has fullscreen window: " + hasFullscreenWindow);
    }

    Component.onCompleted: {
        compositorLogic = Logic.getCompositorInstance();
        if (compositorLogic) {
            compositorLogic.setRootObject(root);
            compositorLogic.setIviScene(iviScene);
            compositorLogic.setDisplaySize(Conf.displayWidth, Conf.displayHeight);
            compositorLogic.init();
        }
    }

    onAddedWaylandSurfaceWindow: {
        var background = Conf.findObjectIdByName("MainMenu");
        var id = compositorLogic.getLayerIdForWaylandSurface("MainMenu");

        if (background == null) {
            console.log("Cannot get background id");
            return;
        }

        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame = windowContainerComponent.createObject(background);

        windowFrame.surface = surface;
        windowFrame.surfaceItem = compositor.item(surface);
        windowFrame.surfaceItem.parent = windowFrame;
        windowFrame.surfaceItem.touchEventsEnabled = true;
        compositorLogic.addWindow(windowFrame, id, true);
    }

    onWidthChanged: {
        Conf.displayWidth = width;
    }
    onHeightChanged: {
        Conf.displayHeight = height;
    }

    function windowDestroyed(surface) {

    }

    function windowAdded(surface) {
        console.log('surface added ' + surface);
        console.log('surface added title:' + surface.title);
        console.log('surface added className:' + surface.className);
        console.log('surface added client: ' + surface.client);
        console.log('surface added pid: ' + surface.client.processId);
        console.log('surface added cmd: ' + util.getCmdForPid(surface.client.processId));
        console.log(iviScene.mainScreen);
        console.log(iviScene.mainScreen.layerCount());

        addedWaylandSurfaceWindow(surface);
    }

    function createQmlComponent(name) {
        var qmlName = name.concat(".qml");

        var windowContainerComponent = Qt.createComponent("WindowFrame.qml", Component.PreferSynchronous);
        var component = Qt.createComponent(qmlName, Component.PreferSynchronous);

        var windowFrame = windowContainerComponent.createObject(root);
        var surface = component.createObject(windowFrame);

        windowFrame.surface = surface;
        windowFrame.surfaceName = name;

        return windowFrame;
    }
}