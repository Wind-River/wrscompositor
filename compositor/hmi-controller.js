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
.import QtQuick 2.0 as QtQuickModule

var Event = {"WindowAdded" : 0, "WindowRemoved" : 1};

var waitProcess = null;
var hmiConroller = null;

var object = function(id, name, handler) {
    this.id = id;
    this.name = name;
}

var Process = function(cmd, object) {
    this.cmd = cmd;
    this.object = object;

    this.getPid = function() {
        return this.object.pid;
    }

    this.launch = function() {
        var pid = this.object.pid;
        if (pid != 0) {
            console.log('no pid');
            return;
        }
        this.object.execute(this.cmd);
    }

    this.quit = function() {
        console.log('quit ' + this.object.pid);
        this.object.quit();
    }
}

function getInstance() {
    if (hmiConroller == null) {
        hmiConroller = new HmiController();
    }

    return hmiConroller;
}

var HmiController = function() {
    this.systemdDbusClient = null;
    this.root = 0;
    this.topWindow = null;
    this.bottomWindow = null;

    this.objectList = new Array();
    this.processList = new Array();

    this.init = function(root) {
        this.root = root;

        this.systemdDbusClient = Qt.createQmlObject(' \
            import QtQuick 2.1; \
            import com.windriver.wrscompositor 1.0; \
            SystemdDbusClient { \
                id: systemd_dbusClient; \
            }',
            this.root, "");
    }

    this.createProcessUnit = function(cmd) {
        var controller = this;
        var object =  Qt.createQmlObject(' \
            import QtQuick 2.1; \
            import com.windriver.wrscompositor 1.0; \
            Process { \
                id: process; \
                property var hmiController: controller; \
                onPidChanged: { \
                    console.log("Process, changed pid"); \
                    if (process.hmiController) \
                        hmiController.waitProcess = process; \
                } \
            }',
            this.root, "");

        var process = new Process(cmd, object);
        this.processList.push(process);
        return process;
    }

    this.registerObjectItem = function(id, name) {
        var newObject = new object(id, name);
        this.objectList.push(newObject);
    }

    this.registerEventHandler = function(handler) {
        this.root.sendEvent.connect(handler);
    }

    this.findObjectByName = function(name) {
        for (var index = 0; index < this.objectList.length; index++) {
            var object = this.objectList[index];
            if (object.name == name) {
                return object.id;
            }
        }
        return null;
    }

    this.createQmlComponent = function(ruleKey, ruleValue) {
        var windowContainerComponent = Qt.createComponent("WindowFrame.qml");

        if (windowContainerComponent.status == QtQuickModule.Component.Error) {
            console.log("createQmlComponent, Error loading component:", windowContainerComponent.errorString());
            return null;
        }

        var windowFrame =
                windowContainerComponent.createObject(
                    this.root,
                    {"x": ruleValue.x,
                     "y": ruleValue.y,
                     "width": ruleValue.width,
                     "height": ruleValue.height,
                     "z": ruleValue.layerId,
                     "opacity": ruleValue.opacity});

        if (windowFrame == null) {
           console.log("createQmlComponent, Error creating object");
           return null;
        }

        var component = Qt.createComponent(ruleKey.concat(".qml"));
        if (component.status == QtQuickModule.Component.Ready) {
            var surface = component.createObject(windowFrame);
            windowFrame.surface = surface;
        } else
            console.log("createQmlComponent, Cannot create QML Component, QML name = ", ruleKey.concat(".qml"));

        windowFrame.name = ruleKey;
        windowFrame.animationsEnabled = ruleValue.animation;

        return windowFrame;
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

        return window;
    }
}