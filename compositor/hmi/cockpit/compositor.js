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

.pragma library

var surfaceRole = { "WRS_IVI_ID_SURFACE_DEFAULT"    : 0,
                    "WRS_IVI_ID_SURFACE_NAVIGATION" : 1,
                    "WRS_IVI_ID_SURFACE_DIALOG"     : 3,
                    "WRS_IVI_ID_SURFACE_PHONE"      : 4,
                    "WRS_IVI_ID_SURFACE_PROJECTION" : 5,
                    "WRS_IVI_ID_SURFACE_CAMERA"     : 6 };

var object = function (name, id) {
    this.name = name;
    this.id = id;
}

var objectList = new Array();

function registerObjectItem (name, id) {
    var newObject = new object(name, id);
    objectList.push(newObject);
}

function findObjectIdByName(name) {
    for (var index = 0; index < objectList.length; index++) {
        var object = objectList[index];
        if (object.name == name) {
            return object.id;
        }
    }
    return null;
}

var compositorLogic = null;
function getCompositorInstance() {
    if (compositorLogic == null) {
        compositorLogic = new Compositor();
    }

    return compositorLogic;
}

var Compositor = function() {
    this.wrscompositor = null;
    this.topWindow = null;
    this.bottomWindow = null;
    this.root = 0;

    this.displayWidth = 0;
    this.displayHeight = 0;

    this.iviScene = null;

    this.windowList = new Array();
    this.compositorRule = new Array();
    this.compositorRole = new Array();

    this.init = function() {
        var request = new XMLHttpRequest();

        request.onreadystatechange = function() {
            if (request.readyState == request.DONE) {
                console.log("onreadystatechange, Done");

                if (request.status == 200) {
                    var compositor = getCompositorInstance();
                    var jsonObject = JSON.parse(request.responseText);

                    var ruleData = jsonObject.Rule;
                    for (var ruleKey in ruleData)
                        compositor.loadCompositorRule(ruleKey, ruleData[ruleKey]);

                    var roleData = jsonObject.Role;
                    for (var roleKey in roleData)
                        compositor.loadCompositorRole(roleKey, roleData[roleKey]);

                    compositor.relayoutWindows();
                }
            }
        }

        request.open("GET", "file:///usr/share/wrscompositor/config.json", true);
        request.send();
    }

    this.loadCompositorRole = function (key, value) {
        console.log("loadCompositorRole, key = ", key, " value = ", value);
        this.compositorRole[key] = value;
    }

    this.loadCompositorRule = function(key, value) {
        console.log("loadCompositionRule, key = ", key, " value = ", value);
        this.compositorRule[key] = value;
    }

    this.setRootObject = function(root) {
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

    this.getLayerIdIByRole = function(role) {
        var roleValue = this.compositorRole[role];
        var ruleValue = this.compositorRule[roleValue];
        return parseInt(ruleValue.split(":")[2]);
    }

    this.getParentWindowByRole = function(role) {
        var parentName = this.compositorRole[role];

        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i].name == parentName)
                return this.windowList[i];
        }

        return null;
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

    this.removeWindow = function(window) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i] == window) {
                this.windowList.splice(i, 1);
                break;
            }
        }
    }

    this.addWindow = function(window) {
        this.windowList.push(window);
    }

    this.createWaylandIviSurface = function(surface, id) {
        var role;
        
        switch (id) {
            case surfaceRole.WRS_IVI_ID_SURFACE_CAMERA:
                role = "Camera";
                break;
            case surfaceRole.WRS_IVI_ID_SURFACE_DIALOG:
                role = "Dialog";
                break;
            case surfaceRole.WRS_IVI_ID_SURFACE_NAVIGATION:
                role = "Navigation";
                break;
            case surfaceRole.WRS_IVI_ID_SURFACE_PHONE:
                role = "Phone";
                break;
            case surfaceRole.WRS_IVI_ID_SURFACE_PROJECTION:
                role = "Projection";
                break;
            case surfaceRole.WRS_IVI_ID_SURFACE_DEFAULT:
                role = "Default";
                break;
            default:
                consoel.log("createWaylandIviSurface, Invalid surface Role");
                return;
        }

        console.log("createWaylandIviSurface, surface's role = ", role);

        var layerId = this.getLayerIdIByRole(role);
        var layer = this.iviScene.mainScreen.layerById(layerId);
        if (layer == null)
            return false;

        var iviSurface = layer.addSurface(0, 0, 0, 0, null, layer, surface);
        var parentItem = this.getParentWindowByRole(role);

        console.log("createWaylandIviSurface, parent's width = ", parentItem.width, " parent's height = ", parentItem.height);
        return parentItem;
    }

    this.addSurface = function (surface) {
        var role = this.wrscompositor.getSurfaceRole(surface);

        if (role == "NotIviSurface")  {
            this.addWaylandSurface(surface, "Default");
        } else {
            this.addWaylandIviSurface(surface, role);
        }
    }

    this.addWaylandSurface = function (surface, role) {
        var parentItem = this.getParentWindowByRole(role);
        var layerId = this.getLayerIdIByRole(role);
        var layer = this.iviScene.mainScreen.layerById(layerId);

        if (parentItem == null) {
            console.log("windowAdded, cannot get parent item");
            return;
        }

        console.log("addWaylandSurface, role = ", role);
        console.log("addWaylandSurface, parentItem's width = ", parentItem.width, " parentItem's height = ", parentItem.height);

        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame = windowContainerComponent.createObject(parentItem);

        windowFrame.surface = surface;
        windowFrame.iviSurface = layer.addSurface(0, 0, surface.size.width, surface.size.height, windowFrame, layer, surface);
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
        windowFrame.targetWidth = parentItem.width;
        windowFrame.targetHeight = parentItem.height;
        windowFrame.scaledWidth = parentItem.width / this.displayWidth;
        windowFrame.scaledHeight = parentItem.height / this.displayHeight;

        this.addWindow(windowFrame);
    }

    this.addWaylandIviSurface = function(surface, role) {
        var parentItem = this.getParentWindowByRole(role);

        if (parentItem == null) {
            console.log("windowAdded, cannot get parent item");
            return;
        }

        console.log("addWaylandIviSurface, role = ", role);
        console.log("addWaylandIviSurface, parentItem's width = ", parentItem.width, " parentItem's height = ", parentItem.height);

        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame = windowContainerComponent.createObject(parentItem);

        windowFrame.surface = surface;
        windowFrame.iviSurface = this.wrscompositor.findIVISurfaceByQWaylandSurface(surface);
        windowFrame.name = role;
        windowFrame.surfaceItem = this.wrscompositor.item(surface);
        windowFrame.surfaceItem.parent = windowFrame;
        windowFrame.surfaceItem.touchEventsEnabled = true;
        windowFrame.iviSurface.setQmlWindowFrame(windowFrame);
        this.addWindow(windowFrame);
    }

    this.destroyWaylandSurface = function(surface) {
       var windowFrame = this.findBySurface(surface);
       if (!windowFrame) {
            console.log("windowDestroyed, cannot find surface in windowList");
            return;
        }

        console.log('window destroyed '+ surface);
        windowFrame.destroy();

        this.removeWindow(windowFrame);
    }

    this.initWindow = function (ruleKey, ruleValue) {
        var sizes = ruleValue.split(":")[0];
        var position = ruleValue.split(":")[1];
        var order = parseInt(ruleValue.split(":")[2]);

        var widthScale = parseInt(sizes.split("x")[0]);
        var heightScale = parseInt(sizes.split("x")[1]);

        var targetWidth = this.displayWidth * widthScale / 100;
        var targetHeight = this.displayHeight * heightScale / 100;

        console.log("initWindow, ruleKey = ", ruleKey);
        console.log("initWindow, position = " + position, " widthScale =", widthScale, " heightScale =", heightScale);
        console.log("initWindow, targetWidth = ", targetWidth, " targetHeight = ", targetHeight);

        var layer = this.addLayer(order);
        var window = this.createQmlComponent(ruleKey, 0, 0, targetWidth, targetHeight, order);
        var iviSurface = layer.addSurface(window.x, window.y, window.width, window.height, window);
        window.iviSurface = iviSurface;

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

            default: {
                console.log("onPositionAligned, Invalid align");
                return;
            }
        }

        this.addWindow(window);

    }

    this.relayoutWindows = function() {
        for (var ruleKey in this.compositorRule) {
            var ruleValue = this.compositorRule[ruleKey];
            this.initWindow(ruleKey, ruleValue)
        }
    }

    this.findBySurface = function(surface) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i].surface == surface)
                return this.windowList[i];
        }
        return null;
    }

    this.createQmlComponent = function(name, x, y, width, height, order) {
        var qmlName = name.concat(".qml");

        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var component = Qt.createComponent(qmlName);

        var windowFrame = windowContainerComponent.createObject(this.root, {"x": x, "y": y, "width": width, "height": height, "z": order});

        var surface = component.createObject(windowFrame);
        windowFrame.surface = surface;
        windowFrame.name = name;

        return windowFrame;
    }

    this.createDynamicItemObject = function(parentItem, width, height, order) {
        var newObject = Qt.createQmlObject('import QtQuick 2.0; Rectangle { width: ('+width+'); height: ('+height+'); z: ('+order+'); color: "#00FFFFFF"}', parentItem, "");
        return newObject;
    }
 }
