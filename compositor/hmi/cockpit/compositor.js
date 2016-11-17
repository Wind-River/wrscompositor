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

var Compositor = function() {
    this.topWindow = null;
    this.bottomWindow = null;

    this.root = 0
    this.currentWidth = 0;
    this.currentHeight = 0;
    this.displayWidth = 0;
    this.displayHeight = 0;

    this.iviScene = null;
   
    this.windowList = new Array();
    this.compositorRules = new Array();

    this.loadCompositionRules = function() {
        // widthScale(percent) : heightScale(percent) : align
        this.compositorRules["StatusBar"] = "100:10:top"; 
        this.compositorRules["DockBar"] = "100:20:bottom";
        this.compositorRules["SidePanel"] = "40:40:middleRight";
        this.compositorRules["MainMenu"] = "50:50:middleLeft";
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

    this.windowPositionToString = function(window) {
        return "[" +
        window.x + ", " +
        window.y + ", " +
        window.width + ", " +
        window.height +
        "]";
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

    this.addSurfacePerLayer = function(id, item) {
        var iviSurface = this.iviScene.createSurface(item.x, item.y, item.width, item.height, item);
        this.iviScene.mainScreen.layerById(id).addSurface(iviSurface);
        this.iviScene.addIVISurface(iviSurface);
    }

    this.windowPositionToString = function (window) {
        return "[" +
            window.x + ", " +
            window.y + ", " +
            window.width + ", " +
            window.height +
        "]";
    }

    /*
     * To be called by compositor root output when an existing compositor element is removed
     * Layout is recalculated
     */
    this.windowRemoved = function(window) {
        /*
        for (var i = 0; i < this.windowList.length; i++) {
            if (this.windowList[i] == window) {
                this.windowList.splice(i, 1);
                break;
            }
        }
        */
    }

    /*
     * To be called by compositor root output when a new compositor element is added
     * Layout is recalculated
     */
    this.windowAdded = function(window) {
        /*
        console.log("[DEBUG] Add window:" + window.subItemName);
        this.windowList.push(window);
        */
    }

    this.updateWindowSize = function (window, ruleValue) {
        var widthScale = parseInt(ruleValue.split(":")[0]);
        var heightScale = parseInt(ruleValue.split(":")[1]);
        var align = ruleValue.split(":")[2];

        var targetHeight = this.currentHeight * heightScale / 100;
        var targetWidth = this.currentWidth * widthScale / 100;

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

        window.sizedChanged(targetWidth, targetHeight);
    }

    this.checkCompositorRule = function(window, windowList, rule) {
        var rules = rule.split("+");

        if (rules.length == 1)  {
            //return false, this "other" case is treated separately
            if ((rules[0] == window.subItemName))
            return (rules[0] == window.subItemName) ? true : false;
        }

        if (rules.length == 2)  {
            if (rules[0] == window.subItemName) {
                if (rules[1] == "other") {
                    return (windowList.length == 1) ? false : true;
                }
                for (var i = 0; i < windowList.length; i++) {
                   if (rules[1] == windowList[i].subItemName)
                        return true;
                }
            }
        }

        return false;
    }

    /*
     * Apply layout algorightm based on:
     * Surfaces preffered size
     * Surfaces preffered position
     * Internal priorities roules
     */
     this.relayoutWindows = function() {
        for (var i = 0; i < this.iviScene.screenCount(); i++) {
            var screen = this.iviScene.screen(i);
            for (var j = 0; j < screen.layerCount(); j++) {
                var layer = screen.layer(j);
                for (var k = 0; k < layer.surfaceCount(); k++) {
                    var surface = layer.surface(k);
                    if (surface != null && surface.qmlWindowFrame() != null) {
                        console.log("surface = ", k, "layer = ", j);
                        this.windowList.push(surface.qmlWindowFrame());
                    }
                }
                this.relayoutWindowPerLayer(this.windowList);
            }
        }
    }

    this.relayoutWindowPerLayer = function(windowList) {
        for (var ruleKey in this.compositorRules) {
            var ruleValue = this.compositorRules[ruleKey];
            console.log("Compositor rule: " + ruleKey + " -> " + ruleValue);
            for (var i = 0; i < windowList.length; i++) {
                var isRuleAccpeted = this.checkCompositorRule(windowList[i], windowList, ruleKey);
                if (isRuleAccpeted)
                    this.updateWindowSize(windowList[i], ruleValue);
            }
        }
    } 
 }