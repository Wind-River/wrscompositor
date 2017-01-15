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

var SurfaceRole = {
    WRS_IVI_ID_SURFACE_DEFAULT: 0,
    WRS_IVI_ID_SURFACE_NAVIGATION: 1,
    WRS_IVI_ID_SURFACE_DIALOG: 2,
    WRS_IVI_ID_SURFACE_PHONE: 3,
    WRS_IVI_ID_SURFACE_PROJECTION: 4,
    WRS_IVI_ID_SURFACE_CAMERA: 5
};

var topWindow = null;
var bottomWindow = null;
var Layout = function(key, value) {
    this.name = key;
    this.scale = value.scale;
    this.position = value.position;
    this.opacity = value.opacity;
    this.layer = value.layer;
    this.animation = value.animation;
    this.x = (value.position == 'undefined') ? value.x : 0;
    this.y = (value.position == 'undefined') ? value.y : 0;
    this.width = 0;
    this.height = 0;
    this.layerId = -1;
    this.surfaceId = -1;

    this.window = null;
    this.background = null;

    this.init = function(background, displayWidth, displayHeight) {
        var widthScale = parseInt(this.scale.split("x")[0]);
        var heightScale = parseInt(this.scale.split("x")[1]);

        this.width = displayWidth * widthScale / 100;
        this.height = displayHeight * heightScale / 100;

        if (this.layer == "base") {
            this.layerId = 1000;
        } else if (this.layer == "application") {
            this.layerId = 2000;
        } else if (this.layer == "home") {
            this.layerId = 3000;
        } else if (this.layer == "notification") {
            this.layerId = 4000;
        } else {
            console.log("init(), layout's layer is invalid")
            return;
        }

        this.surfaceId = this.layerId + 10;
        this.background = background;
    }

    this.getWindow = function() {
        return this.window;
    }

    this.createWindow = function() {
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");

        if (windowContainerComponent.status ==
                                QtQuickModule.Component.Error) {
            console.log("createWindow, Error loading component:",
                                windowContainerComponent.errorString());
            return;
        }

        this.window =
                windowContainerComponent.createObject(
                    this.background,
                    {"x": this.x,
                    "y": this.y,
                    "width": this.width,
                    "height": this.height,
                    "z": this.layerId,
                    "opacity": this.opacity});

        if (this.window == null) {
            console.log("createQmlComponent, Error creating object");
            return;
        }

        var component = Qt.createComponent(this.name.concat(".qml"));
        if (component.status == QtQuickModule.Component.Ready) {
            var surface = component.createObject(this.window);
            this.window.surface = surface;
            console.log("createQmlComponent, create QML Component, QML name = ",
                                            this.name.concat(".qml"));
        }

        this.window.name = this.name;
        this.window.animationsEnabled = this.animation;
    }

    this.layoutWindow = function() {
        switch (this.position) {
            case 'topCenter':
                topWindow = this.window;
                this.window.anchors.top = this.background.top;
                this.window.anchors.horizontalCenter = this.background.horizontalCenter;
                break;
            case 'topLeft':
                topWindow = this.window;
                this.window.anchors.top = this.background.top;
                this.window.anchors.left = this.background.left;
                break;
            case 'topRight':
                topWindow = this.window;
                this.window.anchors.top = this.background.top;
                this.window.anchors.right = this.background.right;
                break;
            case 'bottomCenter':
                bottomWindow = this.window;
                this.window.anchors.bottom = this.background.bottom;
                this.window.anchors.horizontalCenter = this.background.horizontalCenter;
                break;
            case 'bottomLeft':
                bottomWindow = this.window;
                this.window.anchors.bottom = this.background.bottom;
                this.window.anchors.left = this.background.left;
                break;
            case 'bottomRight':
                bottomWindow = this.window;
                this.window.anchors.bottom = this.background.bottom;
                this.window.anchors.right = this.background.right;
                break;
            case 'middleCenter':
                this.window.anchors.horizontalCenter = this.background.horizontalCenter;
                this.window.anchors.verticalCenter = this.background.verticalCenter;
                break;
            case 'middleCenterTop':
                this.window.anchors.top = (topWindow == null) ?
                                      this.background.anchors.top : topWindow.bottom;
                this.window.anchors.horizontalCenter = this.background.horizontalCenter;
                break;
            case 'middleCenterBottom':
                this.window.anchors.bottom = (bottomWindow == null) ?
                                         this.background.bottom : bottomWindow.top;
                this.window.anchors.horizontalCenter = this.background.horizontalCenter;
                break;
            case 'middleLeft':
                this.window.anchors.verticalCenter = this.background.verticalCenter;
                this.window.anchors.left = this.background.left;
                break;
            case 'middleLeftTop':
                this.window.anchors.top = (topWindow == null) ?
                                      this.background.top : topWindow.bottom;
                this.window.anchors.left = this.background.left;
                break;
            case 'middleLeftBottom':
                this.window.anchors.left = this.background.left;
                this.window.anchors.bottom = (bottomWindow == null) ?
                                         this.background.bottom : bottomWindow.top;
                break;
            case 'middleRight':
                this.window.anchors.verticalCenter = this.background.verticalCenter;
                this.window.anchors.right = this.background.right;
                break;
            case 'middleRightTop':
                this.window.anchors.top = (topWindow == null) ?
                                     this.background.top : topWindow.bottom;
                this.window.anchors.right = this.background.right;
                break;
            case 'middleRightBottom':
                this.window.anchors.right = this.background.right;
                this.window.anchors.bottom = (bottomWindow == null) ?
                                         this.background.bottom : bottomWindow.top;
                break;
            case "undefined":
                console.log("layoutWindow, Don't align WindowFrame. WindowFrame is placed by (x,y) coordinate");
                break;
        }
    }
}

var Role = function(key, value) {
    this.name = key;
    this.workspace = value.workspace;
    this.id = value.id;
    this.window = null;

    this.init = function(layoutList) {
        for (var i = 0; i < layoutList.length; i++) {
            var layout = layoutList[i];
            if (layout.name == this.workspace) {
                this.window = layout.getWindow();
                return;
            }
        }
        console.log("Role's init(), \
            Cannot assign Role's window mapped to Workspace");
    }

    this.getWindow = function() {
        return this.window;
    }

    this.getOrder = function() {
        return this.window.z;
    }
}

var WrsLauncher = function(window, list) {
    this.launcherWindow = window;
    this.launcherList = list;

    this.createLauncher = function() {
        var component = Qt.createComponent("wrslauncher.qml");
        if (component.status == QtQuickModule.Component.Ready) {
            var surface = component.createObject(this.launcherWindow,
                                        {"launcherList": this.launcherList});
            this.launcherWindow.surface = surface;
        } else {
            console.log("createLauncherWindow(), Error creating wrslauncher object");
            return;
        }

        console.log("createLauncherWindow, Success to create launcher object");
    }

    this.destroyLauncher = function() {
        console.log("destroyLauncherWindow, Success to destory launcher object");
        this.launcherWindow.surface.destroy();
    }

    this.hideLauncher = function() {
        this.launcherWindow.visible = false;
    }

    this.showLauncher = function() {
        this.launcherWindow.visible = true;
    }

    this.launchNative = function(name) {
        this.launcherWindow.surface.launchNative(name);
    }
}

var WindowList = function() {
    this.defaultWindowList = new Array();
    this.widgetWindowList = new Array();
    this.popupWindowList = new Array();

    this.addDefaultWindow = function(window) {
        console.log("addDefaultWindow, window'name = ", window.name);
        this.defaultWindowList.push(window);
    }

    this.removeDefaultWindow = function(window) {
        for (var i = 0; i < this.defaultWindowList.length; i++) {
            if (this.defaultWindowList[i] == window) {
                console.log("removeDefaultWindow, window'name = ", window.name);
                this.defaultWindowList.splice(i, 1);
                break;
            }
        }
    }

    this.hideDefaultWindow = function(window) {
        for (var i = 0; i < this.defaultWindowList.length; i++) {
            if (this.defaultWindowList[i] == window) {
                this.defaultWindowList[i].visible = false;
                break;
            }
        }
    }

    this.showDefaultWindow = function(window) {
        for (var i = 0; i < this.defaultWindowList.length; i++) {
            if (this.defaultWindowList[i] == window) {
                this.defaultWindowList[i].visible = true;
                break;
            }
        }
    }

    this.getDefaultWindowList = function() {
        return this.defaultWindowList;
    }

    this.addWidgetWindow = function(window) {
        console.log("addWidgetWindow, window'name = ", window.name);
        this.widgetWindowList.push(window);
    }

    this.removeWidgetWindow = function(window) {
        for (var i = 0; i < this.widgetWindowList.length; i++) {
            if (this.widgetWindowList[i] == window) {
                console.log("removeWidgetWindow, window'name = ", window.name);
                this.widgetWindowList.splice(i, 1);
                break;
            }
        }
    }

    this.hideWidgetWindow = function(window) {
        for (var i = 0; i < this.widgetWindowList.length; i++) {
            if (this.widgetWindowList[i] == window) {
                this.widgetWindowList[i].visible = false;
                break;
            }
        }
    }

    this.showWidgetWindow = function(window) {
        for (var i = 0; i < this.widgetWindowList.length; i++) {
            if (this.widgetWindowList[i] == window) {
                this.widgetWindowList[i].visible = true;
                break;
            }
        }
    }

    this.getWidgetWindowList = function() {
        return this.widgetWindowList;
    }

    this.addPopupWindow = function(window) {
        console.log("addPopupWindow, window'name = ", window.name);
        this.popupWindowList.push(window);
    }

    this.removePopupWindow = function(window) {
        for (var i = 0; i < this.popupWindowList.length; i++) {
            if (this.popupWindowList[i] == window) {
                console.log("removePopupWindow, window'name = ", window.name);
                this.popupWindowList.splice(i, 1);
                break;
            }
        }
    }

    this.hidePopupWindow = function(window) {
        for (var i = 0; i < this.popupWindowList.length; i++) {
            if (this.popupWindowList[i] == window) {
                this.popupWindowList[i].visible = false;
                break;
            }
        }
    }

    this.showPopupWindow = function(window) {
        for (var i = 0; i < this.popupWindowList.length; i++) {
            if (this.popupWindowList[i] == window) {
                this.popupWindowList[i].visible = true;
                break;
            }
        }
    }

    this.getPopupWindowList = function() {
        return this.popupWindowList;
    }
}

var compositor = null;
function getInstance() {
    if (compositor == null) {
        compositor = new Compositor();
    }

    return compositor;
}

var Compositor = function() {
    this.wrscompositor = null;

    this.displayWidth = 0;
    this.displayHeight = 0;

    this.root = null;
    this.iviScene = null;
    this.launcher = null;

    this.windowList = new WindowList();
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

                    var layout =  compositor.findLayoutByName("Launcher");
                    var launcherList = jsonObject.Launcher;
                    var launcher = new WrsLauncher(layout.window, launcherList);
                    launcher.createLauncher();
                    compositor.setLauncher(launcher);
                }
            }
        }

        request.open("GET", "file:///usr/share/wrscompositor/config.json", true);
        request.send();
    }

    this.loadCompositorRole = function (key, value) {
        var role = new Role(key, value);
        role.init(this.layoutList);
        this.roleList.push(role);
    }

    this.loadCompositorLayout = function(key, value) {
        var layout = new Layout(key, value);

        layout.init(this.root, this.displayWidth, this.displayHeight);
        layout.createWindow();
        layout.layoutWindow();

        var window = layout.getWindow();
        window.iviSurface = this.addIviLayer(layout.layerId).addSurface(
                                window.x, window.y, window.width, window.height, window, window.surface);
        window.iviSurface.id = layout.surfaceId;

        this.layoutList.push(layout);
    }

    this.getLauncher = function() {
        return this.launcher;
    }

    this.setLauncher = function(launcher) {
        this.launcher = launcher;
    }

    this.setRoot = function(root) {
        this.root = root;
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

    this.createWaylandIviSurface = function(surface, id) {
        var role = this.findSurfaceRoleById(id);

        if (role == null) {
            console.log("createWaylandIviSurface, cannot find surface role related to id");
            return null;
        }

        console.log("createWaylandIviSurface, surface's role = ", role.name);

        var parent = role.getWindow();
        var id = role.getOrder();
        var layer = this.addIviLayer(id);
        if (layer == null)
            return null;

        layer.addSurface(0, 0, parent.width, parent.height, null, surface);
        console.log("createWaylandIviSurface, parent's width = ", parent.width, " parent's height = ", parent.height);
        return parent;
    }

    this.addIviLayer = function(id) {
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

    this.addWaylandSurface = function(surface) {
        var window = null;
        var iviSurface = this.wrscompositor.findIVISurfaceByQWaylandSurface(surface);

        if (iviSurface == null) {
            console.log("addWaylandSurface, surface cannot have iviSurface");
            return null;
        }

        window = this.addWaylandIviSurface(surface, iviSurface.id);
        return window;

    }

    this.addWaylandIviSurface = function(surface, id) {
        var role = this.findSurfaceRoleById(id);
        if (role == null) {
            console.log("windowAdded, cannot get Surface Role by Id");
            return null;
        }

        console.log("addWaylandIviSurface, role = ", role.name);

        var parent = role.getWindow();
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame = windowContainerComponent.createObject(parent);

        windowFrame.rootBackground = parent;
        windowFrame.targetWidth = parent.width;
        windowFrame.targetHeight = parent.height;
        windowFrame.animationsEnabled = parent.animationsEnabled;
        windowFrame.surface = surface;
        windowFrame.iviSurface = this.wrscompositor.findIVISurfaceByQWaylandSurface(surface);
        windowFrame.name = role.name;
        windowFrame.surfaceItem = this.wrscompositor.item(surface);
        windowFrame.surfaceItem.parent = windowFrame;
        windowFrame.surfaceItem.touchEventsEnabled = true;
        windowFrame.iviSurface.setQmlWindowFrame(windowFrame);

        return windowFrame;
    }

    this.hideWindow = function(window) {
        var parent = window.rootBackground;

        switch (parent.name) {
        case 'Default':
            this.windowList.hideDefaultWindow(window);
            break;
        case 'Widget':
            this.windowList.hideWidgetWindow(window);
            break;
        case 'Popup':
            this.windowList.hidePopupWindow(window);
            break;
        default:
        }
    }

    this.showWindow = function(window) {
        var parent = window.rootBackground;

        switch (parent.name) {
        case 'Default':
            this.windowList.showDefaultWindow(window);
            break;
        case 'Widget':
            this.windowList.showWidgetWindow(window);
            break;
        case 'Popup':
            this.windowList.showPopupWindow(window);
            break;
        default:
        }
    }

    this.addWindow = function(window) {
        var parent = window.rootBackground;

        switch (parent.name) {
        case 'Default':
            this.windowList.addDefaultWindow(window);
            break;
        case 'Widget':
            this.windowList.addWidgetWindow(window);
            break;
        case 'Popup':
            this.windowList.addPopupWindow(window);
            break;
        default:
        }
    }

    this.removeWindow = function(window) {
        var parent = window.rootBackground;

        switch (parent.name) {
        case 'Default':
            this.windowList.removeDefaultWindow(window);
            break;
        case 'Widget':
            this.windowList.removeWidgetWindow(window);
            break;
        case 'Popup':
            this.windowList.removePopupWindow(window);
            break;
        default:
        }
    }

    this.resizeDefaultWindow = function(fullsize) {
        var layout = this.findLayoutByName(
            fullsize ? "Default-Plus" : "Default");
        var resizeWidth = layout.getWindow().width;
        var resizeHeight = layout.getWindow().height;
        var defaultWindowList = this.windowList.getDefaultWindowList();

        for (var i = 0; i < defaultWindowList.length; i++) {
            var defaultWindow = defaultWindowList[i];
            if (defaultWindow.animationsEnabled) {
                // When window have the effect of animation,
                // compositor have to scale window's size.
                console.log("resizeDefaultWindow, Window have animation in changing size");
                defaultWindow.scaledWidth =
                                    resizeWidth / defaultWindow.targetWidth;
                defaultWindow.scaledHeight =
                                    resizeHeight / defaultWindow.targetHeight;
            } else {
                // When window don't have the effect of animation,
                // compositor have to send window's size to client
                // and client should configure the window's size
                console.log("resizeDefaultWindow, Window don't have animation in changing size");
                this.wrscompositor.changeIVISurfaceSize(
                    defaultWindow.surface, resizeWidth, resizeHeight);
            }
        }
    }

    this.findSurfaceRoleById = function(id) {
        for (var i = 0; i < this.roleList.length; i++) {
            var role = this.roleList[i];
            if (role.id == id)
                return role;
        }

        return null;
    }

    this.findSurfaceRoleByName = function(name) {
        for (var i = 0; i < this.roleList.length; i++) {
            var role = this.roleList[i];
            if (role.name == name)
                return role;
        }

        return null;
    }

    this.findLayoutByName = function(name) {
        for (var i = 0; i < this.layoutList.length; i++) {
            var layout = this.layoutList[i];
            if (layout.name == name)
                return layout;
        }

        return null;
    }
}