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
    property var compositorLogic : null

    x: 0
    y: 0
    height: windowHeight
    width: windowWidth

    signal sendEvent(int event, var arg);
    signal sendRequest(int request, var arg);

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

        root.sendRequest.connect(requestHandler);
    }

    onWidthChanged: {
        Conf.displayWidth = width;
    }
    onHeightChanged: {
        Conf.displayHeight = height;
    }

    function requestHandler(request, arg) {
        switch(request) {
            case Interface.Request.HideWindow:
            case Interface.Request.ShowWindow:
            {
                var window = arg;
                if (request == Interface.Request.HideWindow) {
                    console.log("requestHandler, HideWindow Request");
                    compositorLogic.hideWindow(window);
                } else {
                    console.log("requestHandler, ShowWindow Request");
                    compositorLogic.showWindow(window);
                }
                break;
            }

            case Interface.Request.LaunchNative:
            case Interface.Request.HideLauncherWindow:
            case Interface.Request.ShowLauncherWindow:
            {
                var launcher = compositorLogic.getLauncher();

                if (request == Interface.Request.LaunchNative) {
                    console.log("requestHandler, LaunchNative Request");
                    var name = arg;
                    launcher.launchNative(name);
                } else if (request == Interface.Request.HideLauncherWindow) {
                    console.log("requestHandler, HideLauncherWindow Request");
                    launcher.hideLauncher();
                } else if (request == Interface.Request.ShowLauncherWindow) {
                    console.log("requestHandler, ShowLauncherWindow Request");
                    launcher.showLauncher();
                }
                break;
            }

            case Interface.Request.ResizeDefaultWindow:
            {
                console.log("requestHandler, ResizeDefaultWindow Request");
                var fullsize = arg;
                if (fullsize) {
                    compositorLogic.hideWindowList("Widget");
                    compositorLogic.hideWindowList("Popup");
                } else {
                    compositorLogic.showWindowList("Widget");
                    compositorLogic.showWindowList("Popup");
                }
                compositorLogic.resizeDefaultWindow(fullsize);
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

        console.log("notify WindowRemoved event");
        root.sendEvent(Interface.Event.WindowRemoved, window);
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

        compositorLogic.addWindow(window);

        console.log("notify WindowAdded event of each qml components for HMI");
        root.sendEvent(Interface.Event.WindowAdded, window);
    }
}