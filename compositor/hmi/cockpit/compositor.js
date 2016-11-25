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

var compositorLogic = null;
function getCompositorInstance() {
    if (compositorLogic == null) {
        compositorLogic = new Compositor();
    }

    return compositorLogic;
}

var Compositor = function() {
    this.topWindow = null;
    this.bottomWindow = null;

    this.root = 0;

    this.currentWidth = 0;
    this.currentHeight = 0;
    this.displayWidth = 0;
    this.displayHeight = 0;

    this.iviScene = null;
 
    this.windowList = new Array();
    this.compositorRules = new Array();

    this.init = function() {
        var request = new XMLHttpRequest();

        request.onreadystatechange = function() {
            if (request.readyState == request.DONE) {
                console.log("onreadystatechange, Done");
                if (request.status == 200) {
                    var compositor = getCompositorInstance();
                    var jsonObject = JSON.parse(request.responseText);
                    var ruleData = jsonObject.Rule;
                    for (var ruleKey in ruleData) {
                        var layerId = parseInt(ruleData[ruleKey].split(":")[2]);
                        compositor.loadCompositorRule(ruleKey, ruleData[ruleKey]);
                        var windowFrame = compositor.createQmlComponent(ruleKey);
                        compositor.addWindow(windowFrame, layerId, false);
                    }

                    compositor.relayoutWindows();
                }
            }
        }

        request.open("GET", "file:///usr/share/wrscompositor/config.json", true);
        request.send();
    }

    this.createQmlComponent = function(name) {
        return this.root.createQmlComponent(name);
    }

    this.loadCompositorRule = function(key, value) {
        console.log("loadCompositionRule, key = ", key, " value = ", value);
        this.compositorRules[key] = value;
    }

    this.getLayerIdForWaylandSurface = function(name) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i].surfaceName == name) {
                return this.windowList[i].layerId;
            }
        }
        return -1;
    }

    this.setRootObject = function(root) {
        this.root = root;
    }

    this.setIviScene = function(iviScene) {
        this.iviScene = iviScene;
    }

    this.setDisplaySize = function(width, height) {
        this.currentHeight = this.displayHeight = height; 
        this.currentWidth = this.displayWidth = width; 
    }

    this.addLayer = function(id) {
        if (this.iviScene.mainScreen.layerById(id) != null) {
            console.log("Layer has already be created. Layer = ", id);
            return;
        }
        this.iviScene.mainScreen.addLayer(id);
        this.iviScene.mainScreen.layer(iviScene.mainScreen.layerCount()-1).visibility = 1;
        this.iviScene.mainScreen.layer(iviScene.mainScreen.layerCount()-1).opacity = 1;
    }

    this.addSurfacePerLayer = function(id, window) {
        var iviSurface = this.iviScene.createSurface(window.x, window.y, window.width, window.height, window);
        this.iviScene.mainScreen.layerById(id).addSurface(iviSurface);
        this.iviScene.addIVISurface(iviSurface);

        window.iviSurface = iviSurface;
        window.layerId = id;
    }

    this.removeWindow = function(window) {
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i] == window) {
                this.windowList.splice(i, 1);
                break;
            }
        }
    }

    this.addWindow = function(window, id, wayland) {
        this.addLayer(id);
        this.addSurfacePerLayer(id, window);
        if (wayland) {
            window.iviSurface.setQWaylandSurface(window.surface); 
            window.surfaceName = iviScene.getSurfaceRole(window.surface);
        }
        this.windowList.push(window);

        console.log("addWindow, name = " + window.surfaceName, " layerId = " + window.layerId);
    }

    this.updateWindow = function (window, ruleValue) {
        var sizes = ruleValue.split(":")[0];
        var align = ruleValue.split(":")[1];

        var widthSize = parseInt(sizes.split("x")[0]);
        var heightSize = parseInt(sizes.split("x")[1]);

        var targetWidth = this.currentWidth * widthSize / 100;
        var targetHeight = this.currentHeight * heightSize / 100;

        console.log("updateWindowSize, name = " + window.surfaceName);
        console.log("updateWindowSize, align = " + align, " widthSize =", widthSize, " heightSize =", heightSize);

        switch (align) {
            case 'top': {
                this.topWindow = window;
                window.positionAligned('top', this.root.top);
                break;
            }

            case 'bottom': {
                this.bottomWindow = window;
                window.positionAligned('bottom', this.root.bottom);
                break;
            }

            case 'middleRight':
            case 'middleLeft': {
                window.positionAligned(
                    (align == 'middleRight') ?  'right' : 'light',
                    (align == 'middleRight') ?   this.root.right : this.root.left
                    );

                if (this.topWindow)
                    window.positionAligned('top', this.topWindow.bottom);
                else if (this.bottomWindow)
                    window.positionAligned('bottom', this.bottomWindow.top);
                
                break;
            }
        }

        window.sizeChanged(targetWidth, targetHeight);
    }

    this.checkCompositorRule = function(window, rule) {
        return (rule == window.surfaceName) ? true : false;
    }

    this.relayoutWindows = function() {
        for (var ruleKey in this.compositorRules) {
            var ruleValue = this.compositorRules[ruleKey];
            for (var i = 0; i < this.windowList.length; i++) {
                var isRuleAccpeted = this.checkCompositorRule(this.windowList[i], ruleKey);
                if (isRuleAccpeted)
                    this.updateWindow(this.windowList[i], ruleValue);
            }
        }
    }
 }