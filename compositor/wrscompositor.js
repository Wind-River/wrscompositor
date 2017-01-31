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
    WRS_IVI_ID_SURFACE_CONNECTIVITY: 4,
    WRS_IVI_ID_SURFACE_CAMERA: 5,
    WRS_IVI_ID_SURFACE_KEYBOARD: 6
};

var Layout = function(key, value) {
    // private member variables
    var name = key;
    var scale = value.scale;
    var position = value.position;
    var opacity = value.opacity;
    var layer = value.layer;
    var animation = value.animation;
    var x = (value.position == 'undefined') ? value.x : 0;
    var y = (value.position == 'undefined') ? value.y : 0;
    var width = 0;
    var height = 0;
    var layerId = -1;
    var surfaceId = -1;

    var window = null;
    var background = null;

    // public member functions
    this.init = function(displayBackground, displayWidth, displayHeight) {
        var widthScale = parseInt(scale.split("x")[0]);
        var heightScale = parseInt(scale.split("x")[1]);

        width = displayWidth * widthScale / 100;
        height = displayHeight * heightScale / 100;

        if (layer == "base") {
            layerId = 1000;
        } else if (layer == "application") {
            layerId = 2000;
        } else if (layer == "home") {
            layerId = 3000;
        } else if (layer == "notification") {
            layerId = 4000;
        } else {
            console.log("init(), layout's layer is invalid")
            return;
        }

        surfaceId = layerId + 10;
        background = displayBackground;
    }

    this.getWindow = function() {
        return window;
    }

    this.getSurfaceId = function() {
        return surfaceId;
    }

    this.getLayerId = function() {
        return layerId;
    }

    this.getName = function() {
        return name;
    }

    this.createWindow = function() {
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");

        if (windowContainerComponent.status ==
                                QtQuickModule.Component.Error) {
            console.log("createWindow, Error loading component:",
                                windowContainerComponent.errorString());
            return;
        }

        window = windowContainerComponent.createObject(
                    background,
                    {"x": x,
                    "y": y,
                    "width": width,
                    "height": height,
                    "z": layerId,
                    "opacity": opacity,
                    "name": name});

        if (window == null) {
            console.log("createQmlComponent, Error creating object");
            return;
        }

        var component = Qt.createComponent(name.concat(".qml"));
        if (component.status == QtQuickModule.Component.Ready) {
            var surface = component.createObject(window);
            window.surface = surface;
            console.log("createQmlComponent, create QML Component, QML name = ",
                                            name.concat(".qml"));
        }

        window.animationsEnabled = animation;
    }

    this.layoutWindow = function() {
        switch (position) {
            case 'topCenter':
                Layout.topWindow = window;
                window.anchors.top = background.top;
                window.anchors.horizontalCenter = background.horizontalCenter;
                break;
            case 'topLeft':
                Layout.topWindow = window;
                window.anchors.top = background.top;
                window.anchors.left = background.left;
                break;
            case 'topRight':
                Layout.topWindow = window;
                window.anchors.top = background.top;
                window.anchors.right = background.right;
                break;
            case 'bottomCenter':
                Layout.bottomWindow = window;
                window.anchors.bottom = background.bottom;
                window.anchors.horizontalCenter = background.horizontalCenter;
                break;
            case 'bottomLeft':
                Layout.bottomWindow = window;
                window.anchors.bottom = background.bottom;
                window.anchors.left = background.left;
                break;
            case 'bottomRight':
                Layout.bottomWindow = window;
                window.anchors.bottom = background.bottom;
                window.anchors.right = background.right;
                break;
            case 'middleCenter':
                window.anchors.horizontalCenter = background.horizontalCenter;
                window.anchors.verticalCenter = background.verticalCenter;
                break;
            case 'middleCenterTop':
                window.anchors.top = (Layout.topWindow == null) ?
                                      background.anchors.top : Layout.topWindow.bottom;
                window.anchors.horizontalCenter = background.horizontalCenter;
                break;
            case 'middleCenterBottom':
                window.anchors.bottom = (Layout.bottomWindow == null) ?
                                         background.bottom : Layout.bottomWindow.top;
                window.anchors.horizontalCenter = background.horizontalCenter;
                break;
            case 'middleLeft':
                window.anchors.verticalCenter = background.verticalCenter;
                window.anchors.left = background.left;
                break;
            case 'middleLeftTop':
                window.anchors.top = (Layout.topWindow == null) ?
                                      background.top : Layout.topWindow.bottom;
                window.anchors.left = background.left;
                break;
            case 'middleLeftBottom':
                window.anchors.left = background.left;
                window.anchors.bottom = (Layout.bottomWindow == null) ?
                                         background.bottom : Layout.bottomWindow.top;
                break;
            case 'middleRight':
                window.anchors.verticalCenter = background.verticalCenter;
                window.anchors.right = background.right;
                break;
            case 'middleRightTop':
                window.anchors.top = (Layout.topWindow == null) ?
                                     background.top : Layout.topWindow.bottom;
                window.anchors.right = background.right;
                break;
            case 'middleRightBottom':
                window.anchors.right = background.right;
                window.anchors.bottom = (Layout.bottomWindow == null) ?
                                         background.bottom : Layout.bottomWindow.top;
                break;
            case "undefined":
                console.log("layoutWindow, Don't align WindowFrame. WindowFrame is placed by (x,y) coordinate");
                break;
        }
    }
}

Layout.topWindow = null;
Layout.bottomWindow = null;

var Role = function(key, value) {
    // private member variables
    var name = key;
    var workspace = value.workspace;
    var id = value.id;
    var window = null;

    // public member functions
    this.init = function(layoutList) {
        for (var i = 0; i < layoutList.length; i++) {
            var layout = layoutList[i];
            var name = layout.getName();
            if (name.indexOf(workspace) != -1) {
                window = layout.getWindow();
                return;
            }
        }
        console.log("Role's init(), \
            Cannot assign Role's window mapped to Workspace");
    }

    this.getWindow = function() {
        return window;
    }

    this.getOrder = function() {
        return window.z;
    }

    this.getRoleId = function() {
        return id;
    }

    this.getRoleName = function() {
        return name;
    }

    this.getWorkspace = function() {
        return workspace;
    }
}

var WrsLauncher = function(window, list) {
    // private member variables
    var launcherWindow = window;
    var launcherList = list;

    // public member functions
    this.createLauncher = function() {
        var component = Qt.createComponent("wrslauncher.qml");
        if (component.status == QtQuickModule.Component.Ready) {
            var surface = component.createObject(launcherWindow,
                                        { "launcherList": launcherList,
                                          "visible": false,
                                          "color": "black"});
            launcherWindow.surface = surface;
        } else {
            console.log("createLauncherWindow(), Error creating wrslauncher object");
            return;
        }

        console.log("createLauncherWindow, Success to create launcher object");
    }

    this.destroyLauncher = function() {
        console.log("destroyLauncherWindow, Success to destory launcher object");
        launcherWindow.surface.destroy();
    }

    this.hideLauncher = function() {
        launcherWindow.surface.visible = false;
    }

    this.showLauncher = function() {
        launcherWindow.surface.visible = true;
    }

    this.launchNative = function(name) {
        launcherWindow.surface.launchNative(name);
    }
}

var WindowManager = function() {
    // private member variables
    var windowList = new Array();
    var currentWindows = new Array();

    // public member functions
    this.init = function(name) {
        windowList[name] = new Array();
    }

    this.addWindow = function(name, window) {
        console.log("addWindow, window'name = ", window.name);
        windowList[name].push(window);
        currentWindows[name] = window;
    }

    this.removeWindow = function(name, window) {
        for (var i = 0; i < windowList[name].length; i++) {
            if (windowList[name][i] == window) {
                console.log("removeWindow, window'name = ", window.name);
                windowList[name].splice(i, 1);
                if (currentWindows[name] == window) {
                    currentWindows[name] = null;
                }
                break;
            }
        }
    }

    this.hideWindow = function(name, window) {
        for (var i = 0; i < windowList[name].length; i++) {
            if (windowList[name][i] == window) {
                if (windowList[name][i].animationsEnabled)
                    windowList[name][i].hideAnimate();
                else
                    windowList[name][i].visible = false;
                break;
            }
        }
    }

    this.showWindow = function(name, window) {
        for (var i = 0; i < windowList[name].length; i++) {
            if (windowList[name][i] == window) {
                if (windowList[name][i].animationsEnabled)
                    windowList[name][i].showAnimate();
                else
                    windowList[name][i].visible = true;
                break;
            }
        }
    }

    this.showWindowList = function(name) {
        for (var i = 0; i < windowList[name].length; i++) {
            if (windowList[name][i].animationsEnabled)
                windowList[name][i].showAnimate();
            else
                windowList[name][i].visible = true;
        }
    }

    this.hideWindowList = function(name) {
        for (var i = 0; i < windowList[name].length; i++) {
            if (windowList[name][i].animationsEnabled)
                windowList[name][i].hideAnimate();
            else
                windowList[name][i].visible = false;
        }
    }

    this.getWindowList = function(name) {
        return windowList[name];
    }

    this.getAllWindowList = function() {
        return windowList;
    }

    this.getCurrentWindow = function(name) {
        return currentWindows[name];
    }

    this.setCurrentWindow = function(name, window) {
        currentWindows[name] = window;
    }

    this.checkCurrentWindow = function (name) {
        return  (currentWindows[name] == null) ? false : true;
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
    // private member variables
    var wrscompositor = null;

    var displayWidth = 0;
    var displayHeight = 0;

    var root = null;
    var iviScene = null;
    var launcher = null;
    var inputPanel = null;

    var windowManager = new WindowManager();
    var layoutList = new Array();
    var roleList = new Array();

    // public member functions
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

                    var launcherList = jsonObject.Launcher;
                    compositor.loadWindowManager();
                    compositor.loadCompositorLauncher(launcherList);

                }
            }
        }

        request.open("GET", "file:///usr/share/wrscompositor/config.json", true);
        request.send();
    }

    this.loadWindowManager = function() {
        for (var i=0 ; i < roleList.length ; i++) {
            var role = roleList[i];
            windowManager.init(role.getWorkspace());
        }
    }

    this.loadCompositorRole = function (key, value) {
        var role = new Role(key, value);
        role.init(layoutList);
        roleList.push(role);
    }

    this.loadCompositorLayout = function(key, value) {
        var layout = new Layout(key, value);

        layout.init(root, displayWidth, displayHeight);
        layout.createWindow();
        layout.layoutWindow();

        var window = layout.getWindow();
        window.iviSurface = addIviLayer(layout.getLayerId()).addSurface(
                                window.x, window.y, window.width, window.height, window, window.surface);
        window.iviSurface.id = layout.getSurfaceId();

        layoutList.push(layout);
    }

    this.loadCompositorLauncher = function(launcherList) {
        var layout = findLayoutByName("Launcher");
        launcher = new WrsLauncher(layout.getWindow(), launcherList);
        launcher.createLauncher();
    }

    this.getLauncher = function() {
        return launcher;
    }

    this.setRoot = function(background) {
        root = background;
    }

    this.setWrsCompositor = function (compositor) {
        wrscompositor = compositor;
    }

    this.setIviScene = function(wrsIviScene) {
        iviScene = wrsIviScene;
    }

    this.setDisplaySize = function(width, height) {
        displayHeight = height;
        displayWidth = width;
    }

    this.createWaylandIviSurface = function(surface, id) {
        var role = findSurfaceRoleById(id);

        if (role == null) {
            console.log("createWaylandIviSurface, cannot find surface role related to id");
            return null;
        }

        console.log("createWaylandIviSurface, surface's role = ", role.getRoleName());

        var parent = role.getWindow();
        var id = role.getOrder();
        var layer = addIviLayer(id);
        if (layer == null)
            return null;

        layer.addSurface(0, 0, parent.width, parent.height, null, surface);
        console.log("createWaylandIviSurface, parent's width = ", parent.width, " parent's height = ", parent.height);
        return parent;
    }

    this.addWaylandSurface = function(surface) {
        var window = null;
        var iviSurface = wrscompositor.findIVISurfaceByQWaylandSurface(surface);

        if (iviSurface == null) {
            console.log("addWaylandSurface, surface cannot have iviSurface");
            return null;
        }

        window = addWaylandIviSurface(surface, iviSurface.id);
        return window;

    }

    this.hideWindowList = function(name)  {
        windowManager.hideWindowList(name);
    }

    this.hideWindow = function(window) {
        var parent = window.rootBackground;

        windowManager.hideWindow(parent.name, window);
    }

    this.raiseWindow = function(window) {
        var parent = window.rootBackground;
        var currentWindow = windowManager.getCurrentWindow(parent.name);

        if (currentWindow != null)
            currentWindow.visible = false;

        window.visible = true;
        windowManager.setCurrentWindow(parent.name, window);
    }

    this.showWindowList = function(name) {
        windowManager.showWindowList(name);
    }

    this.showWindow = function(window) {
        var parent = window.rootBackground;

        windowManager.showWindow(parent.name, window);
    }

    this.addWindow = function(window) {
        var parent = window.rootBackground;

        windowManager.addWindow(parent.name, window);
    }

    this.removeWindow = function(window) {
        var parent = window.rootBackground;

        windowManager.removeWindow(parent.name, window);
    }

    this.showInputPanel = function() {
        var parent = inputPanel.rootBackground;
        console.log("showInputPanel's parent.name = ", parent.name);
        parent.visible = true;
    }

    this.hideInputPanel = function() {
        var parent = inputPanel.rootBackground;
        console.log("hideInputPanel's parent.name = ", parent.name);
        parent.visible = false;
    }

    this.resizeDefaultWindow = function(fullsize) {
        var isDefaultWindowRunning =
            windowManager.checkCurrentWindow("Default");

        if (!isDefaultWindowRunning) {
            console.log("resizeDefaultWindow, there is no default-window on compositor");
            return;
        }

        var layout = findLayoutByName(fullsize ?
                                     "Default-Plus" : "Default");

        var resizeWidth = layout.getWindow().width;
        var resizeHeight = layout.getWindow().height;

        var windowList = windowManager.getAllWindowList();
        for (var name in windowList) {
            if (name == 'Default')
                continue;

            // except for default-window, all of other windows should hide and show.
            if (fullsize) {
                compositorLogic.hideWindowList(name);
            } else {
                compositorLogic.showWindowList(name);
            }
        }

        var defaultWindowList = windowManager.getWindowList("Default");
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
                wrscompositor.changeIVISurfaceSize(
                    defaultWindow.surface, resizeWidth, resizeHeight);
            }
        }
    }

    // private member functions
    function findLayoutByName(name) {
        for (var i = 0; i < layoutList.length; i++) {
            var layout = layoutList[i];
            if (layout.getName() == name)
                return layout;
        }

        return null;
    }

    function findSurfaceRoleById(id) {
        for (var i = 0; i < roleList.length; i++) {
            var role = roleList[i];
            if (role.getRoleId() == id)
                return role;
        }

        return null;
    }

    function findSurfaceRoleByName(name) {
        for (var i = 0; i < roleList.length; i++) {
            var role = roleList[i];
            if (role.getRoleName() == name)
                return role;
        }

        return null;
    }

    function addIviLayer(id) {
        var layer = iviScene.mainScreen.layerById(id);
        if (layer != null) {
            console.log("Layer has already be created. Layer = ", id);
            return layer;
        }

        layer = iviScene.mainScreen.addLayer(id);
        layer.visibility = 1;
        layer.opacity = 1;

        return layer;
    }

    function addWaylandIviSurface(surface, id) {
        var role = findSurfaceRoleById(id);
        if (role == null) {
            console.log("windowAdded, cannot get Surface Role by Id");
            return null;
        }

        console.log("addWaylandIviSurface, role = ", role.getRoleName());

        var parent = role.getWindow();
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame = windowContainerComponent.createObject(parent,
                                    { "opacity": parent.opacity,
                                      "targetWidth": parent.width,
                                      "targetHeight": parent.height,
                                      "animationsEnabled": parent.animationsEnabled,
                                      "rootBackground": parent,
                                      "name": role.getRoleName(),
                                      "roleId": role.getRoleId()});

        windowFrame.surface = surface;
        windowFrame.iviSurface = wrscompositor.findIVISurfaceByQWaylandSurface(surface);
        windowFrame.surfaceItem = wrscompositor.item(surface);
        windowFrame.surfaceItem.parent = windowFrame;
        windowFrame.surfaceItem.touchEventsEnabled = true;
        windowFrame.iviSurface.setQmlWindowFrame(windowFrame);

        if (SurfaceRole.WRS_IVI_ID_SURFACE_KEYBOARD == id &&
            wrscompositor.checkInputPanelSurface(surface)) {
            inputPanel = windowFrame;
            hideInputPanel();
        }

        return windowFrame;
    }

    function raiseWindow(window) {
        if (root.currentWindow != null)
            root.currentWindow.visible = false

        root.currentWindow = window
        root.currentWindow.visible = true
    }
}