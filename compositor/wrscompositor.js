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

var ruleObject = function(ruleValue) {
    this.scale = ruleValue.scale;
    this.position = ruleValue.position;
    this.opacity = ruleValue.opacity;
    this.layer = ruleValue.layer;
    this.animation = ruleValue.animation;
    this.x = (ruleValue.position == 'undefined') ? ruleValue.x : 0;
    this.y = (ruleValue.position == 'undefined') ? ruleValue.y : 0;
    this.width = 0;
    this.height = 0;
    this.layerId = -1;
    this.surfaceId = -1;

    this.setSize = function(displayWidth, displayHeight) {
        var widthScale = parseInt(this.scale.split("x")[0]);
        var heightScale = parseInt(this.scale.split("x")[1]);

        this.width = displayWidth * widthScale / 100;
        this.height = displayHeight * heightScale / 100;
    }

    this.setIdByRule = function() {
        if (this.layer == "base") {
            this.layerId = 1000;
        }
        else if (this.layer == "application") {
            this.layerId = 2000;
        }
        else if (this.layer == "notification") {
            this.layerId = 2000;
        } else {
            console.log("setIdByRule, rule's layer is invalid")
            return;
        }

        this.surfaceId = this.layerId + 10;
    }
}

var roleObject = function(roleValue) {
    this.parent = roleValue.parent;
    this.surfaceId = roleValue.surfaceId;
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
    this.hmiController = null;

    this.displayWidth = 0;
    this.displayHeight = 0;

    this.iviScene = null;

    this.windowList = new Array();
    this.compositorRuleList = new Array();
    this.compositorRoleList = new Array();

    this.init = function() {
        var request = new XMLHttpRequest();

        request.onreadystatechange = function() {
            if (request.readyState == request.DONE) {
                console.log("onreadystatechange, Done");

                if (request.status == 200) {
                    var compositor = getInstance();
                    var jsonObject = JSON.parse(request.responseText);

                    var ruleData = jsonObject.Rule;
                    for (var ruleKey in ruleData)
                        compositor.loadCompositorRule(ruleKey, ruleData[ruleKey]);

                    var roleData = jsonObject.Role;
                    for (var roleKey in roleData)
                        compositor.loadCompositorRole(roleKey, roleData[roleKey]);

                    for (var ruleKey in compositor.compositorRuleList) {
                        var ruleValue = compositor.compositorRuleList[ruleKey];
                        compositor.initWindow(ruleKey, ruleValue)
                    }
                }
            }
        }

        request.open("GET", "file:///usr/share/wrscompositor/config.json", true);
        request.send();
    }

    this.loadCompositorRole = function (key, value) {
        var newRole= new roleObject(value)
        this.compositorRoleList[key] = newRole;
    }

    this.loadCompositorRule = function(key, value) {
        var newRule = new ruleObject(value);

        newRule.setIdByRule();
        newRule.setSize(this.displayWidth, this.displayHeight);

        this.compositorRuleList[key] = newRule;
    }

    this.setHmiController = function(controller) {
        this.hmiController = controller;
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
        var roleValue = this.compositorRoleList[role];
        var parent = roleValue.parent;

        var ruleValue= this.compositorRuleList[parent]
        var layerId = ruleValue.layerId;

        return layerId;
    }

    this.getParentWindowByRole = function(role) {
       var roleValue = this.compositorRoleList[role];
       var parent = roleValue.parent;

        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i].name == parent)
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
        var windowFrame = null;
        var iviSurface = this.wrscompositor.findIVISurfaceByQWaylandSurface(surface);

        if (iviSurface == null)  {
            windowFrame = this.addWaylandSurface(surface, "Default");
        } else {
            windowFrame = this.addWaylandIviSurface(surface,
                this.findSurfaceRoleById(iviSurface.id));
        }

        return windowFrame;
    }

    this.addWaylandSurface = function (surface, role) {
        var parentItem = this.getParentWindowByRole(role);
        var layerId = this.getLayerIdIByRole(role);
        var layer = this.iviScene.mainScreen.layerById(layerId);

        if (parentItem == null) {
            console.log("windowAdded, cannot get parent item");
            return null;
        }

        console.log("addWaylandSurface, role = ", role);
        console.log("addWaylandSurface, parentItem's width = ", parentItem.width, " parentItem's height = ", parentItem.height);

        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");
        var windowFrame = windowContainerComponent.createObject(parentItem);

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
        windowFrame.targetWidth = parentItem.width;
        windowFrame.targetHeight = parentItem.height;
        windowFrame.scaledWidth = parentItem.width / this.displayWidth;
        windowFrame.scaledHeight = parentItem.height / this.displayHeight;

        this.addWindow(windowFrame);

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
        this.addWindow(windowFrame);

        return windowFrame;
    }

    this.initWindow = function (ruleKey, ruleValue) {
        var window = this.hmiController.createQmlComponent(ruleKey, ruleValue);
        if (window == null) {
            console.log("initWindow, Error  creating qml component")
            return;
        }

        window.iviSurface = this.addLayer(ruleValue.layerId).addSurface(window.x, window.y, window.width, window.height, window, window.surface);
        window.iviSurface.id = ruleValue.surfaceId;
        this.addWindow(this.hmiController.layoutWindow(window, ruleValue.position));
    }

    this.findSurfaceRoleById = function(id) {
        for (var roleKey in this.compositorRoleList) {
            var roleValue = this.compositorRoleList[roleKey];
            if (roleValue.surfaceId == id) {
                return roleKey;
            }
        }
        return undefined;
    }

    this.findBySurface = function(surface) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i].surface == surface)
                return this.windowList[i];
        }
        return null;
    }
}
