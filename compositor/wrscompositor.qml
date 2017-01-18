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
import "hmi-interface.js" as Interface
import "config.js" as Conf

Item {
    id: root
    property variant launchedNativeClient: null
    property var compositorLogic : null

    x: 0
    y: 0
    height: windowHeight
    width: windowWidth

    signal notifyEvent(int event, var arg);
    signal sendEvent(int event, var arg);

    property bool hasFullscreenWindow: typeof compositor != "undefined" && compositor.fullscreenSurface !== null

    onHasFullscreenWindowChanged: {
        console.log("has fullscreen window: " + hasFullscreenWindow);
    }

    Component.onCompleted: {
        Interface.wrsCompositor = root;
        compositorLogic = Logic.getInstance();

        if (compositorLogic) {
            compositorLogic.setRoot(root);
            compositorLogic.setIviScene(iviScene);
            compositorLogic.setWrsCompositor(compositor);
            compositorLogic.setDisplaySize(Conf.displayWidth, Conf.displayHeight);
            compositorLogic.init();
        }

        root.sendEvent.connect(eventHandler);
    }

    onWidthChanged: {
        Conf.displayWidth = width;
    }
    onHeightChanged: {
        Conf.displayHeight = height;
    }

    function eventHandler(event, arg) {
        switch(event) {
            case Interface.HMI_EVENT.HIDE_WINDOW:
            case Interface.HMI_EVENT.SHOW_WINDOW:
            case Interface.HMI_EVENT.RAISE_WINDOW:
            {
                var window = arg;
                if (event == Interface.HMI_EVENT.HIDE_WINDOW) {
                    console.log("eventHandler, HIDE_WINDOW Event");
                    compositorLogic.hideWindow(window);
                } else if (event == Interface.HMI_EVENT.SHOW_WINDOW) {
                    console.log("eventHandler, SHOW_WINDOW Event");
                    compositorLogic.showWindow(window);
                } else {
                    console.log("eventHandler, RAISE_WINDOW Event");
                    compositorLogic.raiseWindow(window);
                }
                break;
            }

            case Interface.HMI_EVENT.LAUNCH_NATIVE:
            case Interface.HMI_EVENT.HIDE_LAUNCHER_WINDOW:
            case Interface.HMI_EVENT.SHOW_LAUNCHER_WINDOW:
            {
                var launcher = compositorLogic.getLauncher();

                if (event == Interface.HMI_EVENT.LAUNCH_NATIVE) {
                    console.log("eventHandler, LAUNCH_NATIVE Event");
                    var name = arg;
                    launcher.launchNative(name);
                } else if (event == Interface.HMI_EVENT.HIDE_LAUNCHER_WINDOW) {
                    console.log("eventHandler, HIDE_LAUNCHER_WINDOW Event");
                    launcher.hideLauncher();
                } else if (event == Interface.HMI_EVENT.SHOW_LAUNCHER_WINDOW) {
                    console.log("eventHandler, SHOW_LAUNCHER_WINDOW Event");
                    launcher.showLauncher();
                }
                break;
            }

            case Interface.HMI_EVENT.RESIZE_DEFAULT_WINDOW:
            {
                console.log("eventHandler, RESIZE_DEFAULT_WINDOW Event");
                var fullsize = arg;
                compositorLogic.resizeDefaultWindow(fullsize);
                break;
            }

            case Interface.HMI_EVENT.LAUNCHED_NATIVE:
            {
                console.log("eventHandler, LAUNCHED_NATIVE Event");
                root.launchedNativeClient = arg;
                break;
            }
        }
    }

    function waylandIviSurfaceCreated(surface, id) {
        console.log("surface created, id = ", id);
        return compositorLogic.createWaylandIviSurface(surface, id);
    }

    function windowDestroyed(surface) {
        var surfaceItem = compositor.item(surface);
        var window = surfaceItem.parent;

        if (window == null) {
            console.log("windowDestroyed, Cannot get surface's parent");
            return;
        }

        console.log('window destroyed '+ surface);
        compositorLogic.removeWindow(window);
        window.destroy();

        console.log("notify REMOVE_WINDOW Event");
        root.notifyEvent(Interface.COMPOSITOR_EVENT.REMOVE_WINDOW, window);
    }

    function windowAdded(surface) {
        console.log('surface added ' + surface);
        console.log('surface added title:' + surface.title);
        console.log('surface added className:' + surface.className);
        console.log('surface added width: ' + surface.size.width);
        console.log('surface added height: ' + surface.size.height);
        console.log(iviScene.mainScreen);
        console.log(iviScene.mainScreen.layerCount());

        var window = compositorLogic.addWaylandSurface(surface);
        if (window == null) {
            console.log("windowAdded, cannot add surface in windowFrame");
            return;
        }

        if (root.launchedNativeClient &&
            root.launchedNativeClient.pid == surface.client.processId) {
            root.launchedNativeClient.setWindow(window);
        }

        compositorLogic.addWindow(window);

        console.log("notify ADD_WINDOW Event");
        root.notifyEvent(Interface.COMPOSITOR_EVENT.ADD_WINDOW, window);
    }
}