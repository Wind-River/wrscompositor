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

.import QtQuick 2.0 as QtQuickModule

var layoutObject = function(layoutValue) {
    this.scale = layoutValue.scale;
    this.position = layoutValue.position;
    this.opacity = layoutValue.opacity;
    this.layer = layoutValue.layer;
    this.animation = layoutValue.animation;
    this.x = (layoutValue.position == 'undefined') ? layoutValue.x : 0;
    this.y = (layoutValue.position == 'undefined') ? layoutValue.y : 0;
    this.width = 0;
    this.height = 0;
    this.layerId = -1;
    this.surfaceId = -1;

    this.init = function(displayWidth, displayHeight) {
        var widthScale = parseInt(this.scale.split("x")[0]);
        var heightScale = parseInt(this.scale.split("x")[1]);

        this.width = displayWidth * widthScale / 100;
        this.height = displayHeight * heightScale / 100;

        if (this.layer == "base") {
            this.layerId = 1000;
        } else if (this.layer == "application") {
            this.layerId = 2000;
        } else if (this.layer == "notification") {
            this.layerId = 3000;
        } else if (this.layer == "home") {
            this.layerId = 4000;
        } else {
            console.log("init(), layout's layer is invalid")
            return;
        }

        this.surfaceId = this.layerId + 10;
    }
}

var roleObject = function(roleValue) {
    this.parent = roleValue.parent;
    this.id = roleValue.id;
}

var compositorLogic = null;
function getInstance() {
    if (compositorLogic == null) {
        compositorLogic = new Compositor();
    }

    return compositorLogic;
}

var Compositor = function() {
    this.wrscompositor = null;

    this.displayWidth = 0;
    this.displayHeight = 0;

    this.root = null;
    this.iviScene = null;

    this.topWindow = null;
    this.bottomWindow = null;
    this.launcherWindow = null;

    this.windowList = new Array();
    this.layoutList = new Array();
    this.roleList = new Array();

    this.init = function() {
        var request = new XMLHttpRequest();

        request.onreadystatechange = function() {
            if (request.readyState == request.DONE) {
                console.log("onreadystatechange, Done");

                if (request.status == 200) {
                    var compositor = getInstance();
                    var jsonObject = JSON.parse(request.responseText);
                    var layoutData = jsonObject.Layout;

                    var hmiData = layoutData.Hmi;
                    for (var hmiKey in hmiData)
                        compositor.loadCompositorLayout(hmiKey, hmiData[hmiKey]);

                    var workspaceData = layoutData.Workspace;
                    for (var workspaceKey in workspaceData)
                        compositor.loadCompositorLayout(workspaceKey, workspaceData[workspaceKey]);

                    var roleData = jsonObject.Role;
                    for (var roleKey in roleData)
                        compositor.loadCompositorRole(roleKey, roleData[roleKey]);

                    for (var layoutKey in compositor.layoutList) {
                        var layoutValue = compositor.layoutList[layoutKey];
                        var window = compositor.initWindow(layoutKey, layoutValue);
                        compositor.addWindow(window);
                    }

                    var launcherWindow =  compositor.findWindowByName("Launcher");
                    var launcherList = jsonObject.Launcher;
                    compositor.createLauncherWindow(launcherWindow, launcherList);
                }
            }
        }

        request.open("GET", "file:///usr/share/wrscompositor/config.json", true);
        request.send();
    }

    this.loadCompositorRole = function (key, value) {
        var role = new roleObject(value)
        this.roleList[key] = role;
    }

    this.loadCompositorLayout = function(key, value) {
        var layout = new layoutObject(value);

        layout.init(this.displayWidth, this.displayHeight);
        this.layoutList[key] = layout;
    }

    this.setRoot = function(root) {
        this.root = root;
    }

    this.getLauncherWindow = function() {
        return this.launcherWindow;
    }

    this.setWrsCompositor = function (compositor) {
        this.wrscompositor = compositor;
    }

    this.setIviScene = function(iviScene) {
        this.iviScene = iviScene;
    }

    this.setDisplaySize = function(width, height) {
        this.displayHeight = height;
        this.displayWidth = width;
    }

    this.getLayerIdIByRole = function(role) {
        var roleValue = this.roleList[role];
        var parent = roleValue.parent;

        var layoutValue = this.layoutList[parent]
        var layerId = layoutValue.layerId;

        return layerId;
    }

    this.getParentWindowByRole = function(role) {
        var roleValue = this.roleList[role];
        var parent = roleValue.parent;

        return this.findWindowByName(parent);
    }

    this.addLayer = function(id) {
        var layer = this.iviScene.mainScreen.layerById(id);
        if (layer != null) {
            console.log("Layer has already be created. Layer = ", id);
            return layer;
        }

        layer = this.iviScene.mainScreen.addLayer(id);
        layer.visibility = 1;
        layer.opacity = 1;

        return layer;
    }

    this.createWaylandIviSurface = function(surface, id) {
        var role = this.findSurfaceRoleById(id);

        if (role == undefined) {
            console.log("createWaylandIviSurface, cannot find surface role related to id");
            return null;
        }

        console.log("createWaylandIviSurface, surface's role = ", role);

        var id = this.getLayerIdIByRole(role);
        var layer = this.iviScene.mainScreen.layerById(id);
        if (layer == null)
            return null;

        var parent = this.getParentWindowByRole(role);
        layer.addSurface(0, 0, parent.width, parent.height, null, surface);


        console.log("createWaylandIviSurface, parent's width = ", parent.width, " parent's height = ", parent.height);
        return parent;
    }

    this.addSurface = function (surface) {
        var window = null;
        var iviSurface = this.wrscompositor.findIVISurfaceByQWaylandSurface(surface);

        if (iviSurface == null)  {
            window = this.addWaylandSurface(surface, "Default");
        } else {
            window = this.addWaylandIviSurface(surface,
                this.findSurfaceRoleById(iviSurface.id));
        }

        return window;
    }

    this.addWaylandSurface = function (surface, role) {
        var parent = this.getParentWindowByRole(role);
        var layerId = this.getLayerIdIByRole(role);
        var layer = this.iviScene.mainScreen.layerById(layerId);

        if (parent == null) {
            console.log("windowAdded, cannot get parent");
            return null;
        }

        console.log("addWaylandSurface, role = ", role);
        console.log("addWaylandSurface, parent's width = ", parent.width, " parent's height = ", parent.height);

        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame = windowContainerComponent.createObject(parent);

        windowFrame.surface = surface;
        windowFrame.iviSurface = layer.addSurface(0, 0, surface.size.width, surface.size.height, windowFrame, surface);
        windowFrame.iviSurface.id = surface.client.processId;
        windowFrame.name = role;
        windowFrame.surfaceItem = this.wrscompositor.item(surface);
        windowFrame.surfaceItem.parent = windowFrame;
        windowFrame.surfaceItem.touchEventsEnabled = true;
        windowFrame.iviSurface.setQmlWindowFrame(windowFrame);
        windowFrame.iviSurface.setQWaylandSurface(surface);
        windowFrame.width = surface.size.width;
        windowFrame.height = surface.size.height;
        windowFrame.animationsEnabled = true;
        windowFrame.targetX = 0;
        windowFrame.targetY = 0;
        windowFrame.targetWidth = parent.width;
        windowFrame.targetHeight = parent.height;
        windowFrame.scaledWidth = parent.width / this.displayWidth;
        windowFrame.scaledHeight = parent.height / this.displayHeight;

        return windowFrame;
    }

    this.addWaylandIviSurface = function(surface, role) {
        var parent = this.getParentWindowByRole(role);

        if (parent == null) {
            console.log("windowAdded, cannot get parent window");
            return null;
        }

        console.log("addWaylandIviSurface, role = ", role);
        console.log("addWaylandIviSurface, parent's width = ", parent.width, " parent's height = ", parent.height);

        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame = windowContainerComponent.createObject(parent);

        windowFrame.surface = surface;
        windowFrame.iviSurface = this.wrscompositor.findIVISurfaceByQWaylandSurface(surface);
        windowFrame.name = role;
        windowFrame.surfaceItem = this.wrscompositor.item(surface);
        windowFrame.surfaceItem.parent = windowFrame;
        windowFrame.surfaceItem.touchEventsEnabled = true;
        windowFrame.iviSurface.setQmlWindowFrame(windowFrame);

        return windowFrame;
    }

    this.initWindow = function (layoutKey, layoutValue) {
        var window = this.createQmlComponent(layoutKey, layoutValue);
        if (window == null) {
            console.log("initWindow, Error  creating qml component")
            return null;
        }

        window.iviSurface = this.addLayer(layoutValue.layerId).addSurface(window.x, window.y, window.width, window.height, window, window.surface);
        window.iviSurface.id = layoutValue.surfaceId;
        this.layoutWindow(window, layoutValue.position);

        return window;
    }

    this.layoutWindow = function(window, position) {
        switch (position) {
            case 'topCenter':
            {
                this.topWindow = window;
                window.anchors.top = this.root.top;
                window.anchors.horizontalCenter = this.root.horizontalCenter;
                break;
            }

            case 'topLeft':
            {
                this.topWindow = window;
                window.anchors.top = this.root.top;
                window.anchors.left = this.root.left;
                break;
            }

            case 'topRight':
            {
                this.topWindow = window;
                window.anchors.top = this.root.top;
                window.anchors.right = this.root.right;
                break;
            }

            case 'bottomCenter':
            {
                this.bottomWindow = window;
                window.anchors.bottom = this.root.bottom;
                window.anchors.horizontalCenter = this.root.horizontalCenter;
                break;
            }

            case 'bottomLeft':
            {
                this.bottomWindow = window;
                window.anchors.bottom = this.root.bottom;
                window.anchors.left = this.root.left;
                break;
            }

            case 'bottomRight':
            {
                this.bottomWindow = window;
                window.anchors.bottom = this.root.bottom;
                window.anchors.right = this.root.right;
                break;
            }

            case 'middleCenter':
            {
                window.anchors.horizontalCenter = this.root.horizontalCenter;
                window.anchors.verticalCenter = this.root.verticalCenter;
                break;
            }

            case 'middleCenterTop':
            {
                window.anchors.top = (this.topWindow == null) ?
                                      this.root.anchors.top : this.topWindow.bottom;
                window.anchors.horizontalCenter = this.root.horizontalCenter;
                break;
            }

            case 'middleCenterBottom':
            {
                window.anchors.bottom = (this.bottomWindow == null) ?
                                         this.root.bottom : this.bottomWindow.top;
                window.anchors.horizontalCenter = this.root.horizontalCenter;
                break;
            }

            case 'middleLeft':
            {
                window.anchors.verticalCenter = this.root.verticalCenter;
                window.anchors.left = this.root.left;
                break;
            }

            case 'middleLeftTop':
            {
                window.anchors.top = (this.topWindow == null) ?
                                      this.root.top : this.topWindow.bottom;
                window.anchors.left = this.root.left;
                break;
            }

            case 'middleLeftBottom':
            {
                window.anchors.left = this.root.left;
                window.anchors.bottom = (this.bottomWindow == null) ?
                                         this.root.bottom : this.bottomWindow.top;
                break;
            }

            case 'middleRight':
            {
                window.anchors.verticalCenter = this.root.verticalCenter;
                window.anchors.right = this.root.right;
                break;
            }

            case 'middleRightTop':
            {
                window.anchors.top = (this.topWindow == null) ?
                                     this.root.top : this.topWindow.bottom;
                window.anchors.right = this.root.right;
                break;
            }

            case 'middleRightBottom':
            {
                window.anchors.right = this.root.right;
                window.anchors.bottom = (this.bottomWindow == null) ?
                                         this.root.bottom : this.bottomWindow.top;
                break;
            }

            case "undefined":
            {
                console.log("layoutWindow, Don't align WindowFrame. WindowFrame is placed by (x,y) coordinate");
                break;
            }
        }
    }

    this.hideWindow = function(window) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i] == window) {
                this.windowList[i].visible = false;
                return true;
            }
        }

        return false;
    }

    this.showWindow = function(window) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i] == window) {
                this.windowList[i].visible = true;
                return true;
            }
        }

        return false;
    }

    this.addWindow = function(window) {
        this.windowList.push(window);
    }

    this.removeWindow = function(window) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i] == window) {
                this.windowList.splice(i, 1);
                break;
            }
        }
    }

    this.findWindowBySurface = function(surface) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i].surface == surface)
                return this.windowList[i];
        }
        return null;
    }

    this.findWindowByName = function(name) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i].name == name) {
                return this.windowList[i];
            }
        }

        return null;
    }

    this.findSurfaceRoleById = function(id) {
        for (var roleKey in this.roleList) {
            var roleValue = this.roleList[roleKey];
            if (roleValue.id == id) {
                return roleKey;
            }
        }
        return undefined;
    }

    this.createLauncherWindow = function(launcherWindow, launcherList) {
        var component = Qt.createComponent("wrslauncher.qml");
        if (component.status == QtQuickModule.Component.Ready) {
            var surface = component.createObject(launcherWindow,
                                        {"launcherList": launcherList});
            launcherWindow.surface = surface;
        } else {
            console.log("createLauncherWindow(), Error creating wrslauncher object");
            return;
        }

        this.launcherWindow = launcherWindow;
        console.log("createLauncherWindow, Success to create launcher object");
    }

    this.destroyLauncherWindow = function() {
        console.log("destroyLauncherWindow, Success to destory launcher object");
        this.launcherWindow.surface.destroy();
    }

    this.createQmlComponent = function(layoutKey, layoutValue) {
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");

        if (windowContainerComponent.status ==
                                QtQuickModule.Component.Error) {
            console.log("createQmlComponent, Error loading component:",
                                windowContainerComponent.errorString());
            return null;
        }

        var windowFrame =
                windowContainerComponent.createObject(
                    this.root,
                    {"x": layoutValue.x,
                    "y": layoutValue.y,
                    "width": layoutValue.width,
                    "height": layoutValue.height,
                    "z": layoutValue.layerId,
                    "opacity": layoutValue.opacity});

        if (windowFrame == null) {
            console.log("createQmlComponent, Error creating object");
            return null;
        }

        var component = Qt.createComponent(layoutKey.concat(".qml"));
        if (component.status == QtQuickModule.Component.Ready) {
            var surface = component.createObject(windowFrame);
            windowFrame.surface = surface;
            console.log("createQmlComponent, create QML Component, QML name = ",
                                            layoutKey.concat(".qml"));
        }

        windowFrame.name = layoutKey;
        windowFrame.animationsEnabled = layoutValue.animation;

        return windowFrame;
    }
}